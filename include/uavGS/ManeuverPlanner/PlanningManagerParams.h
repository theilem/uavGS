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

struct PlanningManagerParams
{
	Parameter<std::vector<std::string>> defaultOverrides = {{}, "default_overrides", false};

	template<typename Config>
	inline void
	configure(Config& c)
	{
		c & defaultOverrides;
	}
};


#endif //UAVGS_PLANNINGMANAGERPARAMS_H
