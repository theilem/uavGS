#ifndef GRAPHICSMAPVIEW_H
#define GRAPHICSMAPVIEW_H

#include <uavAP/Core/Frames/VehicleOneFrame.h>
#include <QGraphicsView>
#include <QImage>
#include <QPair>

#include <uavAP/MissionControl/MissionPlanner/Mission.h>
#include <uavAP/MissionControl/GlobalPlanner/PathSections/Curve.h>
#include <uavAP/MissionControl/GlobalPlanner/PathSections/Line.h>
#include <uavAP/MissionControl/GlobalPlanner/PathSections/CubicSpline.h>
#include <uavAP/Core/SensorData.h>
#include <uavGS/MapLogic/MapLogic.h>
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavGS/SensorData/SensorDataManager.h>
#include <uavAP/Core/Frames/LocalFrame.h>
#include <uavAP/Core/DataHandling/DataHandling.h>

class GraphicsMapView : public QGraphicsView, public AggregatableObject<MapLogic, SensorDataManager, DataHandling>
{
Q_OBJECT

	friend class WidgetOverheadMap;

public:

	enum class ViewMode
	{
		FIXED_MODE, FOLLOW_MODE
	};

	explicit
	GraphicsMapView(QWidget* parent = 0);

	Vector3
	mapPointToUTM(QPointF position, double down) const;

	QPointF
	UTMToMapPoint(double e, double n) const;

	QPointF
	LocalFrameToMapPoint(double e, double n) const;

	void
	connect();

protected:

	/**
	 * @brief drawBackground
	 * @param painter
	 * @param rect
	 */
	void
	drawBackground(QPainter* painter, const QRectF& rect) override;

	void
	drawForeground(QPainter* painter, const QRectF&) override;

public slots:

	void
	mouseMoveEvent(QMouseEvent* move) override;

	void
	mouseReleaseEvent(QMouseEvent* event) override;

	void
	mousePressEvent(QMouseEvent* event) override;

private slots:

	void
	contentUpdatedSlot();

signals:

	void
	contentUpdated();

private:

	void
	onSensorData(const SensorData& sd);

	void
	onLocalFrame(const LocalFrame&);

	void
	addLocation(const MapLocation& location);

	void
	drawMap(QPainter* painter, const QRectF& rect);

	void
	drawTrajectory(QPainter* painter);

	void
	drawPathSection(QPainter* painter, std::shared_ptr<IPathSection>, Vector3& lastPoint);

	void
	drawLine(QPainter* painter, std::shared_ptr<Line> line, Vector3& lastPoint);

	void
	drawCurve(QPainter* painter, std::shared_ptr<Curve> curve, Vector3& lastPoint);

	void
	drawCubicSpline(QPainter* painter, std::shared_ptr<CubicSpline> spline, Vector3& lastPoint);

	void
	drawOrbit(QPainter* painter, std::shared_ptr<Orbit> orbit, Vector3& lastPoint);

	void
	drawMission(QPainter* painter);

	void
	drawPathHistory(QPainter* painter);

	void
	drawControllerTarget(QPainter* painter);

	void
	highlightActivePath(QPainter* painter);

	void
	drawAircraft(QPainter* painter);

//	void
//	drawAntenna(QPainter *painter);

	void
	drawSafetyNet(QPainter* painter);

	MapLocation nwCorner_;
	MapLocation seCorner_;
	MapLocation center_;
	MapLocation focus_;
	int zoom_;
	float aircraftScale_;
	QPixmap mapImage_;
	QPixmap aircraftImage_;
	double aircraftHeading_;
	double xPxM_, yPxM_;
	ViewMode viewMode_;
	bool drawWaypoints_;
	bool drawPaths_;
	bool drawSafetyRectangle_;
	MapLocation aircraftLocation_;
	unsigned currentSectionIdx_;

	QPointF moveStart_;
	MapLocation lastCenter_;
	QPointF lastCenterTileCoords_;
	std::vector<MapLocation> pathHistory_;
	unsigned int flightPathSize_;

	LocalFrame localFrame_;
};

#endif // GRAPHICSMAPVIEW_H
