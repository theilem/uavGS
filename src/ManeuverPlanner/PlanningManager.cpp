//
// Created by mirco on 29.06.20.
//

#include "uavGS/ManeuverPlanner/PlanningManager.h"
#include "uavGS/GSWidgetFactory.h"
#include <uavGS/ManeuverPlanner/WidgetManeuverPlanner.h>
#include <uavGS/ManeuverPlanner/ManeuverViewer/WidgetManeuverViewer.h>
#include <uavGS/ParameterSets/WidgetParameterSets.h>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include "uavGS/ParameterSets/WidgetGeneric.h"
#include <uavAP/Core/DataHandling/Content.hpp>
#include <uavAP/FlightControl/LocalPlanner/ManeuverLocalPlanner/ManeuverLocalPlannerParams.h>
#include <uavAP/MissionControl/GlobalPlanner/SplineGlobalPlanner/SplineGlobalPlannerParams.h>
#include <uavAP/MissionControl/LocalFrameManager/LocalFrameManagerParams.h>
#include <uavAP/MissionControl/MissionPlanner/Mission.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/WidgetManeuverEditor.h>

PlanningManager::PlanningManager() : currentManeuverIdx_(-1)
{}

bool
PlanningManager::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSetAll())
			{
				CPSLOG_ERROR << "Missing dependencies";
				return true;
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<WidgetManeuverPlanner>();
			wf->registerWidget<WidgetManeuverViewer>();
			wf->registerWidget<WidgetManeuverEditor>();
//			wf->registerWidget<WidgetGeneric>();
			wf->registerWidget<WidgetParameterSets<ManeuverLocalPlannerParams,
					Content::MANEUVER_LOCAL_PLANNER_PARAMS,
					Target::FLIGHT_CONTROL>>("maneuver_local_planner");
			wf->registerWidget<WidgetParameterSets<SplineGlobalPlannerParams,
					Content::SPLINE_GLOBAL_PLANNER_PARAMS,
					Target::MISSION_CONTROL>>("spline_global_planner");
			wf->registerWidget<WidgetParameterSets<LocalFrameManagerParams,
					Content::LOCAL_FRAME_MANAGER_PARAMS,
					Target::MISSION_CONTROL>>("local_frame_manager");
//			wf->registerWidget<WidgetParameterSets<OverrideSafetyParams,
//					Content::OVERRIDE_SAFETY_PARAMS,
//					Target::FLIGHT_CONTROL>>("override_safety");
			break;
		}
		case RunStage::NORMAL:
		{
			if (auto dh = get<DataHandling>())
			{
				dh->subscribeOnData<ManeuverDescriptor>(Content::MANEUVER, [this](const auto& data)
				{
					currentManeuverSet_ = data;
					onManeuverSet_();
				});
				dh->subscribeOnData<int>(Content::MANEUVER_STATUS, [this](const auto& data)
				{
					currentManeuverIdx_ = data;
					onManeuverStatus_();
				});
				dh->subscribeOnData<std::vector<std::string>>(Content::OVERRIDE_LIST, [this](const auto& data)
				{
					overrideList_ = data;
					onOverrides_();
				});
				dh->subscribeOnData<std::vector<std::string>>(Content::MANEUVER_LIST, [this](const auto& data)
				{
					maneuverList_ = data;
					onManeuvers_();
				});
				dh->subscribeOnData<std::vector<std::string>>(Content::MAINTAIN_LIST, [this](const auto& data)
				{
					maintainList_ = data;
					onMaintains_();
				});
				dh->subscribeOnData<std::map<std::string, Mission>>(Content::MISSION_LIST, [this](const auto& data)
				{
					missionMap_ = data;
					onMission_();
				});
			}
			if (auto sched = get<IScheduler>())
			{
				sched->schedule([this]
								{
									requestAll();
								}, Seconds(1));

			}
			break;
		}
		default:
			break;
	}
	return false;
}

void
PlanningManager::requestCurrentManeuver() const
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(DataRequest::MANEUVER_SET, Content::REQUEST_DATA, Target::FLIGHT_ANALYSIS);
	}
	else
	{
		CPSLOG_ERROR << "Missing DataHandling";
	}
}

void
PlanningManager::requestOverrides() const
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(DataRequest::OVERRIDE_LIST, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
	}
	else
	{
		CPSLOG_ERROR << "Missing DataHandling";
	}
}

void
PlanningManager::requestManeuvers() const
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(DataRequest::MANEUVERS_LIST, Content::REQUEST_DATA, Target::FLIGHT_ANALYSIS);
	}
	else
	{
		CPSLOG_ERROR << "Missing DataHandling";
	}
}

void
PlanningManager::requestMaintains() const
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(DataRequest::MAINTAIN_LIST, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
	}
	else
	{
		CPSLOG_ERROR << "Missing DataHandling";
	}
}

void
PlanningManager::requestMission() const
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(DataRequest::MISSION_LIST, Content::REQUEST_DATA, Target::MISSION_CONTROL);
	}
	else
	{
		CPSLOG_ERROR << "Missing DataHandling";
	}
}

void
PlanningManager::requestAll() const
{
	requestCurrentManeuver();
	requestOverrides();
	requestManeuvers();
	requestMaintains();
	requestMission();
}

std::vector<std::string>
PlanningManager::getDefaultOverrides() const
{
	return params.defaultOverrides();
}

const ManeuverDescriptor&
PlanningManager::getCurrentManeuverSet() const
{
	return currentManeuverSet_;
}

const std::vector<std::string>&
PlanningManager::getOverrides() const
{
	return overrideList_;
}

const std::vector<std::string>&
PlanningManager::getManeuvers() const
{
	return maneuverList_;
}

const std::vector<std::string>&
PlanningManager::getMaintains() const
{
	return maintainList_;
}

const std::map<std::string, Mission>&
PlanningManager::getMission() const
{
	return missionMap_;
}

int
PlanningManager::getCurrentManeuverIdx() const
{
	return currentManeuverIdx_;
}

boost::signals2::connection
PlanningManager::subscribeOnManeuverSet(const boost::signals2::signal<void(void)>::slot_type& slot)
{
	return onManeuverSet_.connect(slot);
}

boost::signals2::connection
PlanningManager::subscribeOnManeuverStatus(const boost::signals2::signal<void(void)>::slot_type& slot)
{
	return onManeuverStatus_.connect(slot);
}

boost::signals2::connection
PlanningManager::subscribeOnOverrides(const boost::signals2::signal<void(void)>::slot_type& slot)
{
	return onOverrides_.connect(slot);
}

boost::signals2::connection
PlanningManager::subscribeOnManeuvers(const boost::signals2::signal<void(void)>::slot_type& slot)
{
	return onManeuvers_.connect(slot);
}

boost::signals2::connection
PlanningManager::subscribeOnMaintains(const boost::signals2::signal<void(void)>::slot_type& slot)
{
	return onMaintains_.connect(slot);
}

boost::signals2::connection
PlanningManager::subscribeOnMission(const boost::signals2::signal<void(void)>::slot_type& slot)
{
	return onMission_.connect(slot);
}
