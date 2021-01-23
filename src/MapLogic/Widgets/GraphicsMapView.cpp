#include <string>
#include <math.h>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QtGui>
#include <QMenu>

#include <uavAP/FlightControl/LocalPlanner/ManeuverLocalPlanner/ManeuverLocalPlannerStatus.h>
#include "uavGS/MapLogic/Widgets/GraphicsMapView.h"

GraphicsMapView::GraphicsMapView(QWidget* parent) :
		QGraphicsView(parent), zoom_(16), drawWaypoints_(false), drawPaths_(false), drawSafetyRectangle_(false)
{
	aircraftHeading_ = 45;
	aircraftScale_ = 1.0;
	aircraftLocation_ = center_;
	viewMode_ = ViewMode::FIXED_MODE;
}

void
GraphicsMapView::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto sdm = get<SensorDataManager>())
	{
		sdm->subscribeOnSensorDataGlobal([this](const auto& sd)
										 { this->onSensorData(sd); });
	}
	if (auto dh = get<DataHandling>())
	{
		dh->subscribeOnData<LocalFrame>(Content::LOCAL_FRAME, [this](const auto& f)
		{ this->onLocalFrame(f); });

		dh->subscribeOnData<ManeuverLocalPlannerStatus>(Content::MANEUVER_LOCAL_PLANNER_STATUS,
														[this](const ManeuverLocalPlannerStatus& status)
														{
															currentSectionIdx_ = status.currentPathSection;
														});
	}
	if (auto ml = get<MapLogic>())
	{
		ml->subscribeOnUpdates([this]
							   { emit contentUpdated(); });
		flightPathSize_ = ml->getParams().flightPathSize();
	}

}

void
GraphicsMapView::drawBackground(QPainter* painter, const QRectF& rect)
{
	//NOTE (0,0) in rect is center of widget, not top left corner
	//painter uses (0,0) as center of widget
	//NOTE (0,0) in viewport()->rect() is top left corner. Thus, viewport()->rect().right() is width and viewport()->rect().bottom() is height
	drawMap(painter, rect);
}

void
GraphicsMapView::drawForeground(QPainter* painter, const QRectF&)
{
	if (drawPaths_)
	{
		drawTrajectory(painter);
		highlightActivePath(painter);
	}
	if (drawWaypoints_)
	{
		drawMission(painter);
	}
	if (drawSafetyRectangle_)
	{
		drawSafetyNet(painter);
	}
	drawControllerTarget(painter);
	drawPathHistory(painter);
	drawAircraft(painter);
}

void
GraphicsMapView::drawMap(QPainter* painter, const QRectF& rect)
{
	painter->setRenderHint(QPainter::Antialiasing);
	if (viewMode_ == ViewMode::FOLLOW_MODE)
	{
		focus_ = aircraftLocation_;    //FOLLOW DOES NOT WORK BECAUSE NEED TO CONVERT UTM TO LAT LON
	}
	else
	{
		focus_ = center_;
	}
	// TILED MAP
	const int TILE_SIZE = 256;
	//const double d = TILE_SIZE*scale;
	const double d = TILE_SIZE;
	int x = (int) floor(focus_.x(zoom_));
	int y = (int) floor(focus_.y(zoom_));
	QPointF focusedTileDraw = -QPointF(d * (focus_.x(zoom_) - x), d * (focus_.y(zoom_) - y));
	QPointF mapDraw = focusedTileDraw;
	QPointF mapBR = focusedTileDraw;
	QPoint nwXY(x, y);
	QPoint seXY(x, y);
	while (mapDraw.x() > rect.left())
	{
		mapDraw.setX(mapDraw.x() - d);
		nwXY.setX(nwXY.x() - 1);
	}
	while (mapDraw.y() > rect.top())
	{
		mapDraw.setY(mapDraw.y() - d);
		nwXY.setY(nwXY.y() - 1);
	}
	while (mapBR.x() < rect.right())
	{
		mapBR.setX(mapBR.x() + d);
		seXY.setX(seXY.x() + 1);
	}
	while (mapBR.y() < rect.bottom())
	{
		mapBR.setY(mapBR.y() + d);
		seXY.setY(seXY.y() + 1);
	}
	QPixmap map(d * (seXY.x() - nwXY.x()), d * (seXY.y() - nwXY.y()));
	QPainter mapPainter(&map);
	mapPainter.fillRect(map.rect(), QColor(0, 0, 0));
	auto mapLogic = get<MapLogic>();
	if (!mapLogic)
	{
		CPSLOG_ERROR << "GraphicsMapView: MapLogic unset!";
		return;
	}
	auto tileDir = mapLogic->getMapTileDirectory();
	for (int i = 0; i < seXY.x() - nwXY.x(); i++)
	{
		for (int j = 0; j < seXY.y() - nwXY.y(); j++)
		{
			auto path = QString::asprintf("%s/%d/%d/%d.jpg", tileDir.c_str(), zoom_, nwXY.x() + i, nwXY.y() + j);
			CPSLOG_TRACE << "tile path is: " << path.toStdString();
			QPixmap tile; // load from tileDirectory or cache
			if (tile.load(path))
			{
				tile = tile.scaled((int) d, (int) d);
				mapPainter.drawPixmap(i * d, j * d, d, d, tile);
			}
		}
	}
	nwCorner_ = MapLocation::fromMapTileCoords(nwXY.x(), nwXY.y(), zoom_);
	seCorner_ = MapLocation::fromMapTileCoords(seXY.x(), seXY.y(), zoom_);
	xPxM_ = map.width() / (seCorner_.easting() - nwCorner_.easting());
	yPxM_ = map.height() / (nwCorner_.northing() - seCorner_.northing());
	QPixmap croppedMap = map;
	painter->drawPixmap(mapDraw, croppedMap);
}

void
GraphicsMapView::drawMission(QPainter* painter)
{
	auto mapLogic = get<MapLogic>();
	if (!mapLogic)
	{
		CPSLOG_ERROR << "GraphicsMapView: MapLogic not set!";
		return;
	}
	painter->setPen(Qt::cyan);
	for (auto wp : mapLogic->getWaypoints())
	{
		QPointF wpLoc = LocalFrameToMapPoint(wp.location().x(), wp.location().y());
		QRectF r;
		r.setCoords(wpLoc.x() - 2, wpLoc.y() - 2, wpLoc.x() + 2, wpLoc.y() + 2);
		painter->fillRect(r, Qt::cyan);
	}
}

void
GraphicsMapView::drawTrajectory(QPainter* painter)
{
	QPen pen(QColor(255, 200, 0));
	pen.setWidth(2);
	painter->setPen(pen);
	auto mapLogic = get<MapLogic>();
	if (!mapLogic)
	{
		CPSLOG_ERROR << "GraphicsMapView: MapLogic not set!";
		return;
	}

//	LocalPlannerStatus status = mapLogic->getLocalPlannerStatus();
	Trajectory traj = mapLogic->getPath();

	if (traj.pathSections.empty())
		return;

	Vector3 lastPoint = traj.pathSections.back()->getEndPoint();

	if (traj.pathSections.size() == 0)
		return;


//	if (status.has_linear_status())
//	{
//		if (status.linear_status().is_in_approach() && traj.approachSection)
//		{
//			drawPathSection(painter, traj.approachSection, lastPoint);
//		}
//	}

	for (const auto& ps : traj.pathSections)
	{
		drawPathSection(painter, ps, lastPoint);
	}
}

void
GraphicsMapView::drawPathHistory(QPainter* painter)
{
	QPointF aircraftCenter = UTMToMapPoint(aircraftLocation_.easting(), aircraftLocation_.northing());
	QPointF first = aircraftCenter;
	QPointF second;
	QColor color = QColor(255, 255, 255);
	auto mapLogic = get<MapLogic>();
	if (!mapLogic)
	{
		CPSLOG_ERROR << "GraphicsMapView: MapLogic not set!";
		return;
	}
	for (unsigned int i = 1; i < pathHistory_.size(); i++)
	{
		float f = 1.0 - (float) (i - 1) / pathHistory_.size();
		color.setAlpha(255 * f);
		second = UTMToMapPoint(pathHistory_[i].easting(), pathHistory_[i].northing());
		painter->setPen(QPen(QBrush(color), 2));
		painter->drawLine(first, second);
		first = second;
	}

}

void
GraphicsMapView::highlightActivePath(QPainter* painter) //highlights active path
{
	QPen pen(Qt::green);
	pen.setWidth(4);
	painter->setPen(pen);
	auto ml = get<MapLogic>();
	if (!ml)
	{
		CPSLOG_ERROR << "Map logic missing. Cannot highlight active path";
		return;
	}

	bool inApproach = false;

	Trajectory traj = ml->getPath();

	if (currentSectionIdx_ >= traj.pathSections.size())
	{
		return;
	}

	std::shared_ptr<IPathSection> ps;
	if (inApproach)
		ps = traj.approachSection;
	else
		ps = traj.pathSections.at(currentSectionIdx_);

	if (ps)
	{
		Vector3 lastPoint(0, 0, 0);
		drawPathSection(painter, ps, lastPoint);
	}
}

void
GraphicsMapView::drawAircraft(QPainter* painter)
{
	// AIRPLANE
	QTransform aircraftTransform;
	aircraftTransform.translate(aircraftImage_.size().width() / 2.0,
								aircraftImage_.size().height() / 2.0);
	aircraftTransform.rotate(-(aircraftHeading_ - M_PI / 2) * 180 / M_PI);
	aircraftTransform.scale(aircraftScale_ / 10.0, aircraftScale_ / 10.0);
	aircraftTransform.translate(aircraftImage_.size().width() / -2.0,
								aircraftImage_.size().height() / -2.0);
	QPixmap rotatedAircraftImage(aircraftImage_.size());
	rotatedAircraftImage.fill(Qt::transparent);
	QPainter aircraftPainter;
	aircraftPainter.begin(&rotatedAircraftImage);
	aircraftPainter.setTransform(aircraftTransform);
	aircraftPainter.drawPixmap(0, 0, aircraftImage_);
	aircraftPainter.end();
	QPointF aircraftCenter = UTMToMapPoint(aircraftLocation_.easting(), aircraftLocation_.northing());
	QPointF halfAircraftSize = QPointF(rotatedAircraftImage.width() / 2.0,
									   rotatedAircraftImage.height() / 2.0);
	QPointF aircraftDrawPoint = aircraftCenter - halfAircraftSize;
	painter->drawPixmap(aircraftDrawPoint, rotatedAircraftImage);
}

void
GraphicsMapView::drawPathSection(QPainter* painter, std::shared_ptr<IPathSection> ps,
								 Vector3& lastPoint)
{
	if (auto curve = std::dynamic_pointer_cast<Curve>(ps))
	{
		drawCurve(painter, curve, lastPoint);
	}
	else if (auto orbit = std::dynamic_pointer_cast<Orbit>(ps))
	{
		drawOrbit(painter, orbit, lastPoint);
	}
	else if (auto spline = std::dynamic_pointer_cast<CubicSpline>(ps))
	{
		drawCubicSpline(painter, spline, lastPoint);
	}
	else if (auto line = std::dynamic_pointer_cast<Line>(ps))
	{
		drawLine(painter, line, lastPoint);
	}
	else
	{
		CPSLOG_WARN << "Unknown PathSection encountered in list during drawTrajectory";
	}
}

void
GraphicsMapView::drawLine(QPainter* painter, std::shared_ptr<Line> line, Vector3& lastPoint)
{
	QPointF first = LocalFrameToMapPoint(line->origin().x(), line->origin().y());
	QPointF second = LocalFrameToMapPoint(line->getEndPoint().x(), line->getEndPoint().y());

	painter->drawLine(first, second);
	lastPoint = line->getEndPoint();
}

void
GraphicsMapView::drawCurve(QPainter* painter, std::shared_ptr<Curve> curve, Vector3& lastPoint)
{
	QPointF center = LocalFrameToMapPoint(curve->getCenter().x(), curve->getCenter().y());
	QPointF toFindRadius = LocalFrameToMapPoint(curve->getCenter().x() + curve->getRadius(),
												curve->getCenter().y() + curve->getRadius());
	double rX = toFindRadius.x() - center.x();
	double rY = -toFindRadius.y() + center.y();
	QRectF rect(center.x() - rX, center.y() - rY, rX * 2, rY * 2);
	QPointF start = LocalFrameToMapPoint(lastPoint.x(), lastPoint.y());
	QPointF end = LocalFrameToMapPoint(curve->getEndPoint().x(), curve->getEndPoint().y());
	double startAngle = 16 * std::atan2(center.y() - start.y(), start.x() - center.x())
						* 180.0 / M_PI; //Qt gives all angles as a multiplier of 16
	double endAngle = 16 * std::atan2(center.y() - end.y(), end.x() - center.x()) * 180.0 / M_PI;
	double spanAngle = endAngle - startAngle;
	if (spanAngle < -16 * 180)
	{
		spanAngle += 16 * 360;
	}
	else if (spanAngle > 16 * 180)
	{
		spanAngle -= 16 * 360;
	}
	painter->drawArc(rect, startAngle, spanAngle);
	lastPoint = curve->getEndPoint();
}

void
GraphicsMapView::drawCubicSpline(QPainter* painter, std::shared_ptr<CubicSpline> spline,
								 Vector3& lastPoint)
{
	const double stepSize = 0.05;
	QPointF first = LocalFrameToMapPoint(spline->c0_.x(), spline->c0_.y());

	Vector2 c0 = spline->c0_.head(2);
	Vector2 c1 = spline->c1_.head(2);
	Vector2 c2 = spline->c2_.head(2);
	Vector2 c3 = spline->c3_.head(2);

	for (double u = stepSize; u <= 1; u += stepSize)
	{
		auto p = c0 + c1 * u + c2 * pow(u, 2) + c3 * pow(u, 3);
		QPointF next = LocalFrameToMapPoint(p.x(), p.y());
		painter->drawLine(first, next);
		first = next;
	}
	lastPoint = spline->getEndPoint();
}

void
GraphicsMapView::drawOrbit(QPainter* painter, std::shared_ptr<Orbit> orbit, Vector3& lastPoint)
{
	QPointF orbitCenter = LocalFrameToMapPoint(orbit->getCenter().x(), orbit->getCenter().y());
	QPointF toFindRadius = LocalFrameToMapPoint(orbit->getCenter().x() + orbit->getRadius(),
												orbit->getCenter().y() + orbit->getRadius());
	double rX = sqrt(pow(toFindRadius.x() - orbitCenter.x(), 2) + pow(toFindRadius.y() - orbitCenter.y(), 2)) / sqrt(2);
	painter->drawEllipse(orbitCenter, rX, rX);
	lastPoint = orbit->getCenter();
}

void
GraphicsMapView::onLocalFrame(const LocalFrame& frame)
{
	localFrame_ = frame;
	int zone = center_.getZone();
	char hemi = center_.getHemi();
	center_ = MapLocation::fromVector3(localFrame_.origin, zone, hemi);
	emit contentUpdated();
}

void
GraphicsMapView::drawSafetyNet(QPainter* painter)
{
	auto mapLogic = get<MapLogic>();

	if (!mapLogic)
	{
		CPSLOG_ERROR << "GraphicsMapView: MapLogic Missing.";
		return;
	}

	const auto& bounds = mapLogic->getSafetyBounds();

	const auto& boundsCenter = bounds.center();
	double rotation = bounds.majorSideOrientation() * M_PI / 180.0;
	double majorOffset = bounds.majorSideLength() / 2;
	double minorOffset = bounds.minorSideLength() / 2;

	Vector2 Q1 = rotate2Drad(Vector2(majorOffset, minorOffset), rotation) + boundsCenter.head(2);
	Vector2 Q2 = rotate2Drad(Vector2(-majorOffset, minorOffset), rotation) + boundsCenter.head(2);
	Vector2 Q3 = rotate2Drad(Vector2(-majorOffset, -minorOffset), rotation) + boundsCenter.head(2);
	Vector2 Q4 = rotate2Drad(Vector2(majorOffset, -minorOffset), rotation) + boundsCenter.head(2);

	QPointF M1 = LocalFrameToMapPoint(Q1.x(), Q1.y());
	QPointF M2 = LocalFrameToMapPoint(Q2.x(), Q2.y());
	QPointF M3 = LocalFrameToMapPoint(Q3.x(), Q3.y());
	QPointF M4 = LocalFrameToMapPoint(Q4.x(), Q4.y());

	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(2);

	painter->setPen(pen);
	painter->drawLine(M1, M2);
	painter->drawLine(M2, M3);
	painter->drawLine(M3, M4);
	painter->drawLine(M1, M4);
}

void
GraphicsMapView::drawControllerTarget(QPainter* painter)
{
	auto mapLogic = get<MapLogic>();
	if (!mapLogic)
	{
		CPSLOG_ERROR << "GraphicsMapView: MapLogic not set!";
		return;
	}
//	auto status = mapLogic->getLocalPlannerStatus();
//	if (!status.has_linear_status())
//		return;
////	if (status.linear_status().has_airplane_status())
//	{
//		//drawing controller target
//
//		double command = status.linear_status().airplane_status().heading_target();
//		QPointF airplaneLoc = LocalFrameToMapPoint(aircraftLocation.easting(),
//				aircraftLocation.northing()); //ENU
//		QPointF dest = QPointF(airplaneLoc.x() + 50 * cos(-command + M_PI / 2),
//				airplaneLoc.y() - 50 * sin(-command + M_PI / 2));
//		QPen pen;
//		pen.setColor(Qt::red);
//		pen.setWidth(3);
//		painter->setPen(pen);
//		painter->drawLine(airplaneLoc, dest);
//
//		//drawing position deviation
//		auto pathIndex = status.linear_status().current_path_section();
//		if (mapLogic->getPath().pathSections.size() <= pathIndex)
//		{
//			return;
//		}
//
//		std::shared_ptr<IPathSection> ps;
//		if (status.linear_status().is_in_approach())
//			ps = mapLogic->getPath().approachSection;
//		else
//			ps = mapLogic->getPath().pathSections.at(pathIndex);
//		Vector3 currentPos = xyzTypeToVector3(mapLogic->getSensorData().position);
//		ps->updatePosition(currentPos);
//
//		Vector3 closestPoint = currentPos + ps->getPositionDeviation();
//		QPointF mapPoint = LocalFrameToMapPoint(closestPoint[0], closestPoint[1]);
//		pen.setColor(Qt::blue);
//		pen.setWidth(2);
//		painter->setPen(pen);
//		painter->drawLine(airplaneLoc, mapPoint);
//		pen.setWidth(5);
//		painter->setPen(pen);
//		painter->drawPoint(mapPoint);
//	}
}

Vector3
GraphicsMapView::mapPointToUTM(QPointF position, double down) const
{
	QPointF diff = position - this->rect().center();
	double northing = focus_.northing() - diff.y() / yPxM_;
	double easting = focus_.easting() + diff.x() / xPxM_;
	return Vector3(northing, easting, down);
}

QPointF
GraphicsMapView::UTMToMapPoint(double e, double n) const
{
	QPointF mapCenter = QPointF((focus_.easting() - nwCorner_.easting()) * xPxM_,
								(nwCorner_.northing() - focus_.northing()) * yPxM_);
	float x = (e - nwCorner_.easting()) * xPxM_ - mapCenter.x();
	float y = (nwCorner_.northing() - n) * yPxM_ - mapCenter.y();
	return QPointF(x, y);
}

QPointF
GraphicsMapView::LocalFrameToMapPoint(double e, double n) const
{
	Vector3 local(e, n, 0);
	local = local + localFrame_.origin;
	return UTMToMapPoint(local.x(), local.y());
}

void
GraphicsMapView::onSensorData(const SensorData& sd)
{
	aircraftLocation_ = MapLocation(sd.position.x(), sd.position.y());
	addLocation(aircraftLocation_);
	aircraftHeading_ = sd.attitude.z();
	emit contentUpdated();
}

void
GraphicsMapView::addLocation(const MapLocation& location)
{
	if (pathHistory_.empty())
	{
		for (unsigned i = 0; i < flightPathSize_; i++)
		{
			pathHistory_.push_back(location);
		}
	}
	else
	{
		pathHistory_.pop_back();
		pathHistory_.insert(pathHistory_.begin(), location);
	}
}

void
GraphicsMapView::mouseMoveEvent(QMouseEvent* move)
{
	if (move->buttons() & Qt::LeftButton) {
		QPointF newCenter = lastCenterTileCoords_ - (move->localPos() - moveStart_) / 256.0;
		center_ = MapLocation::fromMapTileCoords(newCenter.x(), newCenter.y(), zoom_);
		viewport()->update();
	}
}

void
GraphicsMapView::mousePressEvent(QMouseEvent* event)
{
	moveStart_ = event->localPos();
	lastCenter_ = center_;
	lastCenterTileCoords_ = QPointF(lastCenter_.x(zoom_), lastCenter_.y(zoom_));
}

void
GraphicsMapView::mouseReleaseEvent(QMouseEvent* event)
{
}

void
GraphicsMapView::contentUpdatedSlot()
{
	viewport()->update();
}
