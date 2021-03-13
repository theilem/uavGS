//
// Created by mirco on 29.06.20.
//

#ifndef UAVGS_PLANNINGMANAGER_H
#define UAVGS_PLANNINGMANAGER_H

#include <uavAP/FlightAnalysis/ManeuverPlanner/Maneuver.h>

#include "uavGS/ManeuverPlanner/PlanningManagerParams.h"
#include <cpsCore/cps_object>
#include <boost/signals2/signal.hpp>

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

	void
	requestCurrentManeuver() const;

	std::vector<std::string>
	getDefaultOverrides() const;

	const std::vector<ManeuverOverride>&
	getCurrentManeuverSet() const;

	using OnManeuverOverrides = boost::signals2::signal<void(const std::vector<ManeuverOverride>&)>;


	boost::signals2::connection
	subscribeOnManeuverSet(const OnManeuverOverrides::slot_type& slot);


private:
	std::vector<ManeuverOverride> currentManeuverSet_;

	OnManeuverOverrides onManeuverSet_;
};


#endif //UAVGS_PLANNINGMANAGER_H
