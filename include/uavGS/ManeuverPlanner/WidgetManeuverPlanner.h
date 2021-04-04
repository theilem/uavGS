#ifndef WIDGETMANEUVERPLANNER_H
#define WIDGETMANEUVERPLANNER_H


#include <cpsCore/Aggregation/AggregatableObject.hpp>

#include <QWidget>
#include <uavAP/MissionControl/MissionPlanner/Mission.h>
#include "uavGS/ParameterSets/NamedLineEdit.h"
#include "uavGS/ParameterSets/NamedCheckbox.h"
#include <uavAP/Core/DataHandling/DataHandling.h>
#include "uavGS/ParameterSets/NamedLineEdit.h"

namespace Ui
{
class WidgetManeuverPlanner;
}
class PlanningManager;

class WidgetManeuverPlanner: public QWidget, public AggregatableObject<DataHandling, PlanningManager, IScheduler>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "maneuver_planner";

	explicit
	WidgetManeuverPlanner(QWidget* parent = nullptr);
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

	void
	on_addOverride_clicked();

	void
	on_removeOverride_clicked();

	void
	on_defaultsButton_clicked();

private slots:

	void
	contentUpdatedSlot();

signals:

	void
	contentUpdated();

private:

	void
	addOverrideWidget(const std::string& id);

	Ui::WidgetManeuverPlanner* ui;

	std::map<std::string, NamedLineEdit*> overrides_;
};

#endif // WIDGETMANEUVERPLANNER_H
