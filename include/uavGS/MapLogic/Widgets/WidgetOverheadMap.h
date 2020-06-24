#ifndef WIDGETOVERHEADMAP_H
#define WIDGETOVERHEADMAP_H

#include <QWidget>

#include <uavAP/MissionControl/GlobalPlanner/PathSections/IPathSection.h>

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavGS/MapLogic/MapLocation.h>
#include <uavGS/MapLogic/MapLogic.h>
#include "uavGS/MapLogic/Widgets/GraphicsMapView.h"
#include "uavGS/GSWidgetFactory.h"

namespace Ui
{
class WidgetOverheadMap;
}


class WidgetOverheadMap : public QWidget, public AggregatableObject<MapLogic, GSWidgetFactory>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "overhead_map";

	explicit
	WidgetOverheadMap(QWidget* parent = nullptr);

	~WidgetOverheadMap();

	void
	connect();

	void
	setMode(GraphicsMapView::ViewMode mode);

	void
	setMapImage(const QPixmap& map);

	void
	setNWMapCorner(const MapLocation& loc);

	void
	setSEMapCorner(const MapLocation& loc);

	void
	setMapCenter(const MapLocation& loc);

	void
	setAirplaneLocation(const MapLocation& loc);

	void
	setAirplaneHeading(double heading);

	void
	requestGraphicsUpdate();

private slots:

	void
	on_fixedModeButton_toggled(bool checked);

	void
	on_displayWaypoints_toggled(bool checked);

	void
	on_displayTrajectory_toggled(bool checked);

	void
	on_displaySafetyRectangle_toggled(bool checked);

	void
	on_requestMission_clicked();

	void
	on_requestTrajectory_clicked();

	void
	on_airplaneScaleBox_valueChanged(double arg1);

	void
	on_zoomBox_valueChanged(double arg1);

	void
	on_updateCenter_clicked();

	void
	on_requestSafetyNet_clicked();

private:

	Ui::WidgetOverheadMap* ui;
};

#endif // WIDGETOVERHEADMAP_H
