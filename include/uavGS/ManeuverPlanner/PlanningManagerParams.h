//
// Created by mirco on 29.06.20.
//

#ifndef UAVGS_PLANNINGMANAGERPARAMS_H
#define UAVGS_PLANNINGMANAGERPARAMS_H

#include <cpsCore/Configuration/Parameter.hpp>
#include <uavAP/FlightControl/LocalPlanner/LocalPlannerTargets.h>
#include <uavAP/FlightControl/Controller/ControllerTarget.h>
#include <uavAP/FlightControl/Controller/PIDController/PIDMapping.h>
#include <uavAP/FlightControl/Controller/ControllerOutput.h>
#include <uavAP/FlightControl/Controller/ControllerConstraint.h>
#include <uavAP/MissionControl/ManeuverPlanner/Override.h>

struct PlanningManagerParams
{
//	Parameter<std::vector<LocalPlannerTargets>> localPlannerTargets = {{}, "local_planner_targets", false};
//	Parameter<std::vector<ControllerTargets>> controllerTargets = {{}, "controller_targets", false};
//	Parameter<std::vector<PIDs>> pidTargets = {{}, "pid_targets", false};
//	Parameter<std::vector<ControllerOutputs>> controllerOutputs = {{}, "controller_outputs", false};
//	Parameter<std::vector<ControllerConstraints>> controllerConstraints = {{}, "controller_constraints", false};
//	Parameter<std::vector<CustomOverrideIDs>> customOverrides = {{}, "custom_overrides", false};

	template<typename Config>
	inline void
	configure(Config& c)
	{
	//		c & localPlannerTargets;
	//		c & controllerTargets;
	//		c & pidTargets;
	//		c & controllerOutputs;
	//		c & controllerConstraints;
	//		c & customOverrides;
	}
};


#endif //UAVGS_PLANNINGMANAGERPARAMS_H
