//
// Created by mirco on 29.06.20.
//

#include "uavGS/ManeuverPlanner/PlanningManager.h"
#include "uavGS/GSWidgetFactory.h"
#include <uavGS/ManeuverPlanner/WidgetManeuverPlanner.h>
#include <uavGS/ParameterSets/WidgetParameterSets.h>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include "uavGS/ParameterSets/WidgetGeneric.h"
#include <uavAP/Core/DataHandling/Content.hpp>
#include <uavAP/FlightControl/LocalPlanner/ManeuverLocalPlanner/ManeuverLocalPlannerParams.h>
#include <uavAP/MissionControl/GlobalPlanner/SplineGlobalPlanner/SplineGlobalPlannerParams.h>
#include <uavAP/MissionControl/LocalFrameManager/LocalFrameManagerParams.h>
#include <uavAP/FlightControl/Safety/OverrideSafetyParams.h>

bool
PlanningManager::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSet<DataHandling, GSWidgetFactory>())
			{
				CPSLOG_ERROR << "Missing dependencies";
				return true;
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<WidgetManeuverPlanner>();
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
			break;
		}
		default:
			break;
	}
	return false;
}

std::vector<std::string>
PlanningManager::getDefaultOverrides() const
{
	return params.defaultOverrides();
}
