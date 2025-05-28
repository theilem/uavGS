//
// Created by mirco on 29.06.20.
//

#ifndef UAVGS_PLANNINGMANAGER_H
#define UAVGS_PLANNINGMANAGER_H

#include "uavGS/ManeuverPlanner/PlanningManagerParams.h"
#include <cpsCore/cps_object>

#include "uavAP/Core/DataHandling/Content.hpp"

template <typename C, typename T>
class DataHandling;
class GSWidgetFactory;

class PlanningManager
		: public AggregatableObject<DataHandling<Content, Target>, GSWidgetFactory>,
		  public ConfigurableObject<PlanningManagerParams>,
		  public IRunnableObject
{
public:

	static constexpr auto typeId = "planning_manager";

	bool
	run(RunStage stage) override;

	std::vector<std::string>
	getDefaultOverrides() const;
};


#endif //UAVGS_PLANNINGMANAGER_H
