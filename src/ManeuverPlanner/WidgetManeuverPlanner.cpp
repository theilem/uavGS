#include <vector>
#include <string>
#include <QJsonArray>
#include <uavGS/ManeuverPlanner/WidgetManeuverPlanner.h>
#include <uavGS/ManeuverPlanner/PlanningManager.h>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/ManeuverPlanner/VisualScripter/SelectionValue.h>

#include "ui_WidgetManeuverPlanner.h"


WidgetManeuverPlanner::WidgetManeuverPlanner(QWidget* parent) :
		QWidget(parent), ui(new Ui::WidgetManeuverPlanner)
{
	ui->setupUi(this);
}

WidgetManeuverPlanner::~WidgetManeuverPlanner()
{
	delete ui;
}
//
//void
//WidgetManeuverPlanner::connectInterface(std::shared_ptr<IWidgetInterface> interface)
//{
//	if (!interface)
//	{
//		APLOG_ERROR << "WidgetManeuverPlanner cannot connect to interface";
//		return;
//	}
//	if (!interface->getConfigManager().isSet())
//	{
//		APLOG_ERROR << "WidgetManeuverPlanner cannot get ConfigManager";
//		return;
//	}
//	configManager_.set(interface->getConfigManager().get());
//	ui->missionOptions->addItem("default");
//	if (configManager_.isSet())
//	{
//		auto cm = configManager_.get();
//		PropertyMapper<Configuration> pm(cm->getMissionConfig());
//		Configuration planner;
//		Configuration maneuver;
//		if (pm.add("mission_planner", planner, true))
//		{
//			PropertyMapper<Configuration> plannerPm(planner);
//			Configuration missions;
//			if (plannerPm.add("missions", missions, true))
//			{
//				for (const auto& it : missions)
//				{
//					ui->missionOptions->addItem(QString::fromStdString(it.first));
//				}
//			}
//		}
//		if (pm.add("maneuver_planner", maneuver, true))
//		{
//			PropertyMapper<Configuration> plannerPm(maneuver);
//			Configuration maneuvers;
//			if (plannerPm.add("maneuvers", maneuvers, true))
//			{
//				for (const auto& it : maneuvers)
//				{
//					ui->maneuverOptions->addItem(QString::fromStdString(it.first));
//				}
//			}
//		}
//
//		auto config = cm->getWidgetConfigByName(widgetName);
//		configure(config);
//	}
//}

void
WidgetManeuverPlanner::on_apply_clicked()
{
	ManeuverOverride overrides;
	for (const auto& override: overrides_)
	{
		if (const auto& opVal = override.second->get(); opVal.has_value())
		{
			overrides.insert(opVal.value());
		}
	}
	auto dh = get<DataHandling>();

	dh->sendData(overrides, Content::OVERRIDE, Target::FLIGHT_CONTROL);
}

void
WidgetManeuverPlanner::on_abort_clicked()
{
	auto dh = get<DataHandling>();

	dh->sendData(ManeuverOverride(), Content::OVERRIDE, Target::FLIGHT_CONTROL);
	dh->sendData(true, Content::ABORT_MANEUVER, Target::FLIGHT_ANALYSIS);
}

void
WidgetManeuverPlanner::on_sendManeuver_clicked()
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(ui->maneuverOptions->currentText().toStdString(), Content::SELECT_MANEUVER_SET,
					 Target::FLIGHT_ANALYSIS);
	}
}

void
WidgetManeuverPlanner::on_sendMission_clicked()
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(ui->missionOptions->currentText().toStdString(), Content::SELECT_MISSION, Target::MISSION_CONTROL);
	}
}

void
WidgetManeuverPlanner::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto dh = get<DataHandling>())
	{
		dh->subscribeOnData<std::map<std::string, Mission>>(Content::MISSION_LIST, [this](const auto& m)
		{ emit contentUpdated(); });
		dh->subscribeOnData<std::vector<std::string>>(Content::OVERRIDE_LIST, [this](const auto& m)
		{ emit contentUpdated(); });
		dh->subscribeOnData<std::vector<std::string>>(Content::MANEUVER_LIST, [this](const auto& m)
		{ emit contentUpdated(); });
	}
	if (auto sched = get<IScheduler>())
	{
		sched->schedule([this]
						{
							on_update_clicked();
						}, Seconds(1));

	}
}

void
WidgetManeuverPlanner::on_update_clicked()
{
	if (auto pm = get<PlanningManager>())
	{
		pm->requestAll();
	}
}

void
WidgetManeuverPlanner::contentUpdatedSlot()
{
	if (auto pm = get<PlanningManager>())
	{
		auto missionMap = pm->getMission();
		ui->missionOptions->clear();
		for (const auto& it : missionMap)
		{
			ui->missionOptions->addItem(QString::fromStdString(it.first));
		}
		auto overrideList = pm->getOverrides();
		ui->overrideList->clear();
		for (const auto& it : overrideList)
		{
			ui->overrideList->addItem(QString::fromStdString(it));
		}
		auto maneuversList = pm->getManeuvers();
		ui->maneuverOptions->clear();
		for (const auto& it : maneuversList)
		{
			ui->maneuverOptions->addItem(QString::fromStdString(it));
		}
	}
}


void
WidgetManeuverPlanner::addOverrideWidget(const std::string& id)
{
	// NOTE: copy pasted and adapted from WidgetMotionPrimitive. We should make a common override class to do this
	auto sv = new SelectionValue(id, this);
	QObject::connect(sv, &SelectionValue::buttonClicked, this, &WidgetManeuverPlanner::onOverrideDeleteClicked);
	const auto& insertItr = overrides_.insert_or_assign(id, sv);
	int insertionIdx = std::distance(overrides_.begin(), insertItr.first);
	ui->overrides->insertWidget(insertionIdx, sv);
}


void
WidgetManeuverPlanner::on_addOverride_clicked()
{
	auto id = ui->overrideList->currentText().toStdString();
	if (overrides_.find(id) != overrides_.end())
		return;
	addOverrideWidget(id);
}

void
WidgetManeuverPlanner::on_defaultsButton_clicked()
{
	auto pm = get<PlanningManager>();
	if (!pm)
	{
		CPSLOG_ERROR << "Planning manager missing";
		return;
	}
	auto defaultOverrides = pm->getDefaultOverrides();

	for (auto& it : overrides_)
	{
		ui->overrides->removeWidget(it.second);
		delete it.second;
	}
	overrides_.clear();
	auto overrideList = pm->getOverrides();
	for (const auto& override : defaultOverrides)
	{
		if (std::find(overrideList.begin(), overrideList.end(), override) != overrideList.end())
			addOverrideWidget(override);
		else
			CPSLOG_WARN << "Override " << override << " not available in autopilot";
	}
}

void
WidgetManeuverPlanner::onOverrideDeleteClicked(const std::string& key, SelectionValue* wid)
{
	wid = overrides_[key];
	overrides_.erase(key);
	ui->overrides->removeWidget(wid);
	delete wid;
}

