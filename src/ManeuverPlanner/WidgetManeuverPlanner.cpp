#include <vector>
#include <string>
#include <QJsonArray>
#include <uavGS/ManeuverPlanner/WidgetManeuverPlanner.h>
#include <uavGS/ManeuverPlanner/PlanningManager.h>
#include <uavAP/Core/DataHandling/DataHandling.h>

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
	std::map<std::string, FloatingType> overrides;
	for (const auto&[id, edit]: overrides_)
	{
		if (edit->isEmpty())
			continue;
		overrides.insert(std::make_pair(id, edit->getDouble()));
	}
	auto dh = get<EnumBasedDataHandling>();

	dh->sendData(overrides, Content::OVERRIDE, Target::FLIGHT_CONTROL);
}

void
WidgetManeuverPlanner::on_abort_clicked()
{
	auto dh = get<EnumBasedDataHandling>();

	dh->sendData(std::map<std::string, FloatingType>(), Content::OVERRIDE, Target::FLIGHT_CONTROL);
	dh->sendData(true, Content::ABORT_MANEUVER, Target::FLIGHT_ANALYSIS);
}

void
WidgetManeuverPlanner::on_sendManeuver_clicked()
{
	if (auto dh = get<EnumBasedDataHandling>())
	{
		dh->sendData(ui->maneuverOptions->currentText().toStdString(), Content::SELECT_MANEUVER_SET,
					 Target::FLIGHT_ANALYSIS);
	}
}

void
WidgetManeuverPlanner::on_sendMission_clicked()
{
	if (auto dh = get<EnumBasedDataHandling>())
	{
		dh->sendData(ui->missionOptions->currentText().toStdString(), Content::SELECT_MISSION, Target::MISSION_CONTROL);
	}
	else
	{
		CPSLOG_ERROR << "DataHandling missing, cannot send mission";
	}

	if (auto sched = get<IScheduler>())
	{
		sched->schedule([this]
		{
			auto dh = get<EnumBasedDataHandling>();
			if (dh)
			{
				dh->sendData(DataRequest::MISSION, Content::REQUEST_DATA, Target::MISSION_CONTROL);
			}
		}, Seconds(1));
		sched->schedule([this]
		{
			auto dh = get<EnumBasedDataHandling>();
			if (dh)
			{
				dh->sendData(DataRequest::TRAJECTORY, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
			}
		}, Seconds(2));
	}

}

void
WidgetManeuverPlanner::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto dh = get<EnumBasedDataHandling>())
	{
		dh->subscribeOnData<std::vector<std::string>>(Content::MISSION_LIST, [this](const auto& m)
		{ missionList_ = m; emit contentUpdated(); });
		dh->subscribeOnData<std::vector<std::string>>(Content::OVERRIDE_LIST, [this](const auto& m)
		{ overrideList_ = m; emit contentUpdated(); });
		dh->subscribeOnData<std::vector<std::string>>(Content::MANEUVER_LIST, [this](const auto& m)
		{ maneuversList_ = m; emit contentUpdated(); });
	}
}

void
WidgetManeuverPlanner::on_update_clicked()
{
	if (auto dh = get<EnumBasedDataHandling>())
	{
		dh->sendData(DataRequest::MISSION_LIST, Content::REQUEST_DATA, Target::MISSION_CONTROL);
		dh->sendData(DataRequest::OVERRIDE_LIST, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
		dh->sendData(DataRequest::MANEUVERS_LIST, Content::REQUEST_DATA, Target::FLIGHT_ANALYSIS);
	}
}

void
WidgetManeuverPlanner::contentUpdatedSlot()
{
	ui->missionOptions->clear();
	for (const auto& it : missionList_)
	{
		ui->missionOptions->addItem(QString::fromStdString(it));
	}

	ui->overrideList->clear();
	for (const auto& it : overrideList_)
	{
		ui->overrideList->addItem(QString::fromStdString(it));
	}

	ui->maneuverOptions->clear();
	for (const auto& it : maneuversList_)
	{
		ui->maneuverOptions->addItem(QString::fromStdString(it));
	}
}


void
WidgetManeuverPlanner::addOverrideWidget(const std::string& id)
{
	auto lv = new NamedLineEdit(id, this);
	ui->overrides->addWidget(lv);
	overrides_.insert(std::make_pair(id, lv));
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
WidgetManeuverPlanner::on_removeOverride_clicked()
{
	auto it = overrides_.find(ui->overrideList->currentText().toStdString());
	if (it == overrides_.end())
		return;

	ui->overrides->removeWidget(it->second);
	delete it->second;
	overrides_.erase(it);
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

	for (const auto& override : defaultOverrides)
	{
		if (std::find(overrideList_.begin(), overrideList_.end(), override) != overrideList_.end())
			addOverrideWidget(override);
		else
			CPSLOG_WARN << "Override " << override << " not available in autopilot";
	}
}

