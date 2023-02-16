
#include "ui_WidgetOverheadMap.h"
#include <QJsonDocument>
#include <QtOpenGL/QGLWidget>
#include <uavGS/MapLogic/Widgets/WidgetOverheadMap.h>
#include <QtWidgets/QGraphicsView>

//#include "QMouseEvent"
WidgetOverheadMap::WidgetOverheadMap(QWidget* parent) :
		QWidget(parent), ui(new Ui::WidgetOverheadMap)
{
	ui->setupUi(this);
	ui->airplaneScaleLabel->setStyleSheet("color:white;");
	ui->zoomLabel->setStyleSheet("color:white;");
	ui->mapView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	ui->mapView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	ui->mapView->setScene(new QGraphicsScene());

	//ui->mapView->antenna = MapLocation(); Maybe reimplement
	ui->mapView->center_ = MapLocation();
	ui->mapView->nwCorner_ = MapLocation();
	ui->mapView->seCorner_ = MapLocation();

	ui->displayWaypoints->setCheckState(Qt::Checked);
	ui->displayTrajectory->setCheckState(Qt::Checked);
	ui->displaySafetyRectangle->setCheckState(Qt::Checked);
	ui->mapView->drawWaypoints_ = true;
	ui->mapView->drawPaths_ = true;
	ui->mapView->drawSafetyRectangle_ = true;
	ui->mapView->viewport()->update();
}

WidgetOverheadMap::~WidgetOverheadMap()
{
	//https://doc.qt.io/qt-5/qgraphicsview.html#setScene -- view does not take ownership of scene so it must be deleted
	delete ui->mapView->scene();
	delete ui;
}

void
WidgetOverheadMap::connect()
{
	if (auto ml = get<MapLogic>())
	{
		QPixmap temp;
		std::string aircraftImagePath = ml->getIconPath() + "airplane.png";
		temp.load(QString::fromStdString(aircraftImagePath));
		ui->mapView->aircraftImage_ = temp;
		ui->mapView->center_ = ml->getMapCenter();
		ui->CenterX->setText(QString::number(ui->mapView->center_.latitude()));
		ui->CenterY->setText(QString::number(ui->mapView->center_.longitude()));
	}
	if (auto wf = get<GSWidgetFactory>())
	{
		wf->connectWidget(ui->mapView);
	}
}

void
WidgetOverheadMap::setMapImage(const QPixmap& map)
{
	ui->mapView->mapImage_ = map;
}

void
WidgetOverheadMap::setNWMapCorner(const MapLocation& loc)
{
	ui->mapView->nwCorner_ = loc;
}

void
WidgetOverheadMap::setSEMapCorner(const MapLocation& loc)
{
	ui->mapView->seCorner_ = loc;
}

void
WidgetOverheadMap::setMapCenter(const MapLocation& loc)
{
	ui->mapView->center_ = loc;
}

void
WidgetOverheadMap::setAirplaneHeading(double heading)
{
	ui->mapView->aircraftHeading_ = heading;
}

void
WidgetOverheadMap::setMode(GraphicsMapView::ViewMode mode)
{
	ui->mapView->viewMode_ = mode;
}

void
WidgetOverheadMap::requestGraphicsUpdate()
{
	this->update();
	ui->mapView->viewport()->update();
}

void
WidgetOverheadMap::on_fixedModeButton_toggled(bool checked)
{
	ui->mapView->viewMode_ = (
			(checked) ?
			GraphicsMapView::ViewMode::FIXED_MODE : GraphicsMapView::ViewMode::FOLLOW_MODE);
	ui->mapView->viewport()->update();
}

void
WidgetOverheadMap::on_displayWaypoints_toggled(bool checked)
{
	ui->mapView->drawWaypoints_ = checked;
	ui->mapView->viewport()->update();
}

void
WidgetOverheadMap::on_displayTrajectory_toggled(bool checked)
{
	ui->mapView->drawPaths_ = checked;
	ui->mapView->viewport()->update();
}

void
WidgetOverheadMap::on_displaySafetyRectangle_toggled(bool checked)
{
	ui->mapView->drawSafetyRectangle_ = checked;
	ui->mapView->viewport()->update();
}

void
WidgetOverheadMap::on_requestMission_clicked()
{
	if (auto ml = get<MapLogic>())
	{
		ml->askForMission();
		ml->askForLocalFrame();
	}
}

void
WidgetOverheadMap::on_requestTrajectory_clicked()
{
	if (auto ml = get<MapLogic>())
	{
		ml->askForTrajectory();
		ml->askForLocalFrame();
	}
}

void
WidgetOverheadMap::on_airplaneScaleBox_valueChanged(double arg1)
{
	ui->mapView->aircraftScale_ = arg1;
	QPainter a(ui->mapView->viewport());
	ui->mapView->drawAircraft(&a);
}

void
WidgetOverheadMap::on_zoomBox_valueChanged(double arg1)
{
	ui->mapView->zoom_ = (int) arg1;
	ui->mapView->viewport()->update();
}

void
WidgetOverheadMap::on_updateCenter_clicked()
{
	ui->mapView->center_ = MapLocation::fromLatLong(ui->CenterX->text().toDouble(),
												   ui->CenterY->text().toDouble());
	ui->mapView->viewport()->update();
}

void
WidgetOverheadMap::on_requestSafetyNet_clicked()
{
	if (auto ml = get<MapLogic>())
	{
		ml->askForSafetyNet();
		ml->askForLocalFrame();
	}
}
