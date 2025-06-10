﻿#ifndef WIDGETMANEUVERPLANNER_H
#define WIDGETMANEUVERPLANNER_H


#include <cpsCore/Aggregation/AggregatableObject.hpp>

#include <QWidget>
#include <uavAP/MissionControl/MissionPlanner/Mission.h>

#include "uavAP/Core/DataHandling/Content.hpp"
#include "uavGS/ParameterSets/NamedLineEdit.h"
#include "uavGS/ParameterSets/NamedCheckbox.h"
#include "uavGS/ParameterSets/NamedLineEdit.h"

namespace Ui
{
class WidgetManeuverPlanner;
}
class PlanningManager;
class IScheduler;
template <typename C, typename T>
class DataHandling;

class WidgetManeuverPlanner: public QWidget, public AggregatableObject<DataHandling<Content, Target>, PlanningManager, IScheduler>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "maneuver_planner";

	explicit
	WidgetManeuverPlanner(QWidget* parent = nullptr);

	~WidgetManeuverPlanner() override;

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

	std::vector<std::string> missionList_;
	std::map<std::string, NamedLineEdit*> overrides_;
	std::vector<std::string> overrideList_;
	std::vector<std::string> maneuversList_;
};

#endif // WIDGETMANEUVERPLANNER_H
