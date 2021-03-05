//
// Created by mirco on 29.06.20.
//

#ifndef UAVGS_PLANNINGMANAGER_H
#define UAVGS_PLANNINGMANAGER_H

#include "uavGS/ManeuverPlanner/PlanningManagerParams.h"
#include <cpsCore/cps_object>

class DataHandling;
class GSWidgetFactory;

class PlanningManager
		: public AggregatableObject<DataHandling, GSWidgetFactory>,
		  public ConfigurableObject<PlanningManagerParams>,
		  public IRunnableObject
{
public:

	static constexpr TypeId typeId = "planning_manager";

	bool
	run(RunStage stage) override;

	std::vector<std::string>
	getDefaultOverrides() const;
};


#endif //UAVGS_PLANNINGMANAGER_H
