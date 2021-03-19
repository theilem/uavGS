//
// Created by mirco on 29.06.20.
//

#ifndef UAVGS_PLANNINGMANAGER_H
#define UAVGS_PLANNINGMANAGER_H

#include <uavAP/FlightAnalysis/ManeuverPlanner/Maneuver.h>

#include "uavGS/ManeuverPlanner/PlanningManagerParams.h"
#include <cpsCore/cps_object>
#include <boost/signals2/signal.hpp>

class IScheduler;
class DataHandling;
class GSWidgetFactory;

class PlanningManager
		: public AggregatableObject<DataHandling, GSWidgetFactory, IScheduler>,
		  public ConfigurableObject<PlanningManagerParams>,
		  public IRunnableObject
{

public:

	static constexpr TypeId typeId = "planning_manager";

	using OnManeuverOverrides = boost::signals2::signal<void(const ManeuverDescriptor&)>;

	using OnManeuverStatus = boost::signals2::signal<void(const unsigned int&)>;

	bool
	run(RunStage stage) override;

	void
	requestCurrentManeuver() const;

	std::vector<std::string>
	getDefaultOverrides() const;

	const ManeuverDescriptor&
	getCurrentManeuverSet() const;

	boost::signals2::connection
	subscribeOnManeuverSet(const OnManeuverOverrides::slot_type& slot);

	boost::signals2::connection
	subscribeOnManeuverStatus(const OnManeuverStatus::slot_type& slot);

	unsigned int
	getCurrentManeuverIdx() const;

private:
	ManeuverDescriptor currentManeuverSet_;
	OnManeuverOverrides onManeuverSet_;
	OnManeuverStatus onManeuverStatus_;
	unsigned int currentManeuverIdx_;
};


#endif //UAVGS_PLANNINGMANAGER_H
