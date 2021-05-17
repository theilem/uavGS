#ifndef WIDGETMANEUVERPLANNER_H
#define WIDGETMANEUVERPLANNER_H


#include <map>
#include <QWidget>

#include <cpsCore/Aggregation/AggregatableObject.hpp>
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
class SelectionValue;

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
	on_defaultsButton_clicked();

private slots:

	void
	contentUpdatedSlot();

	void
	onOverrideDeleteClicked(const std::string& key, SelectionValue* wid);

signals:

	void
	contentUpdated();

private:

	void
	addOverrideWidget(const std::string& id);

	std::map<std::string, SelectionValue *> overrides_;

	Ui::WidgetManeuverPlanner* ui;
};

#endif // WIDGETMANEUVERPLANNER_H
