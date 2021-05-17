//
// Created by mirco on 29.06.20.
//

#ifndef UAVGS_PLANNINGMANAGER_H
#define UAVGS_PLANNINGMANAGER_H

#include <uavAP/FlightAnalysis/ManeuverPlanner/Maneuver.h>

#include "uavGS/ManeuverPlanner/PlanningManagerParams.h"
#include <cpsCore/cps_object>
#include <boost/signals2/signal.hpp>
#include <uavGS/ManeuverPlanner/VisualScripter/Standalone/IOverridesProvider.h>

class IScheduler;
class DataHandling;
class GSWidgetFactory;
class Mission;

class PlanningManager
		: public AggregatableObject<DataHandling, GSWidgetFactory, IScheduler>,
		  public ConfigurableObject<PlanningManagerParams>,
		  public IRunnableObject,
		  public IOverridesProvider
{

public:

	static constexpr TypeId typeId = "planning_manager";

	PlanningManager();

	bool
	run(RunStage stage) override;

	void
	requestCurrentManeuver() const;

	void
	requestOverrides() const;

	void
	requestManeuvers() const;

	void
	requestMaintains() const;

	void
	requestMission() const;

	void
	requestAll() const;

	std::vector<std::string>
	getDefaultOverrides() const;

	const ManeuverDescriptor&
	getCurrentManeuverSet() const;

	const std::vector<std::string>&
	getOverrides() const override;

	const std::vector<std::string>&
	getManeuvers() const;

	const std::vector<std::string>&
	getMaintains() const override;

	const std::map<std::string, Mission>&
	getMission() const;

	int
	getCurrentManeuverIdx() const;

	boost::signals2::connection
	subscribeOnManeuverSet(const boost::signals2::signal<void(void)>::slot_type& slot);

	boost::signals2::connection
	subscribeOnManeuverStatus(const boost::signals2::signal<void(void)>::slot_type& slot);

	boost::signals2::connection
	subscribeOnOverrides(const boost::signals2::signal<void(void)>::slot_type& slot);

	boost::signals2::connection
	subscribeOnManeuvers(const boost::signals2::signal<void(void)>::slot_type& slot);

	boost::signals2::connection
	subscribeOnMaintains(const boost::signals2::signal<void(void)>::slot_type& slot);

	boost::signals2::connection
	subscribeOnMission(const boost::signals2::signal<void(void)>::slot_type& slot);

private:
	ManeuverDescriptor currentManeuverSet_;
	std::vector<std::string> overrideList_;
	std::vector<std::string> maneuverList_;
	std::vector<std::string> maintainList_;
	// Note, the mission isn't actually used, I suggest just saving the mission names
	std::map<std::string, Mission> missionMap_;
	int currentManeuverIdx_;

	boost::signals2::signal<void(void)> onManeuverSet_;
	boost::signals2::signal<void(void)> onOverrides_;
	boost::signals2::signal<void(void)> onManeuvers_;
	boost::signals2::signal<void(void)> onMaintains_;
	boost::signals2::signal<void(void)> onMission_;
	boost::signals2::signal<void(void)> onManeuverStatus_;
};


#endif //UAVGS_PLANNINGMANAGER_H
