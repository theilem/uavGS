#ifndef WIDGETMANEUVERPLANNER_H
#define WIDGETMANEUVERPLANNER_H


#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/MissionControl/ManeuverPlanner/Override.h>

#include <QWidget>
#include <uavAP/MissionControl/MissionPlanner/Mission.h>
#include "uavGS/ParameterSets/NamedLineEdit.h"
#include "uavGS/ParameterSets/NamedCheckbox.h"
#include <uavAP/Core/DataHandling/DataHandling.h>

namespace Ui
{
class WidgetManeuverPlanner;
}

class WidgetManeuverPlanner: public QWidget, public AggregatableObject<DataHandling>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "maneuver_planner";

	explicit
	WidgetManeuverPlanner(QWidget* parent = 0);
	~WidgetManeuverPlanner();

	void
	connect();


private slots:
	void
	on_apply_clicked();

	void
	on_abort_clicked();

	void
	on_sendManeuver_clicked();

	void
	on_sendMission_clicked();

	void
	on_update_clicked();

private slots:

	void
	contentUpdatedSlot();

signals:

	void
	contentUpdated();

private:
	Ui::WidgetManeuverPlanner* ui;

	std::map<std::string, Mission> missionMap_;

	std::map<LocalPlannerTargets, NamedLineEdit*> localPlannerTargets_;
	std::map<ControllerTargets, NamedLineEdit*> controllerTargets_;
	std::map<PIDs, NamedLineEdit*> pids_;
	std::map<ControllerOutputs, NamedLineEdit*> controllerOutputs_;
	std::map<ControllerConstraints, NamedLineEdit*> controllerConstraints_;
	std::map<CustomOverrideIDs, NamedLineEdit*> custom_;
};

#endif // WIDGETMANEUVERPLANNER_H
