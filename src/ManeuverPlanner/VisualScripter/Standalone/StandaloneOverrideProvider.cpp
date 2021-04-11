//
// Created by seedship on 4/9/21.
//

#include "uavGS/ManeuverPlanner/VisualScripter/Standalone/StandaloneOverrideProvider.h"
#include <uavAP/FlightControl/FlightControlHelper.h>
#include <cpsCore/Synchronization/SimpleRunner.h>

const std::vector<std::string>&
StandaloneOverrideProvider::getOverrides() const
{
	return overrides_;
}

const std::vector<std::string>&
StandaloneOverrideProvider::getMaintains() const
{
	return maintains_;
}

void
StandaloneOverrideProvider::loadConfig(const std::string& path)
{

	CPSLOG_DEBUG << "Initializing FlightControl";
	auto agg = FlightControlHelper::createAggregation(path);

	SimpleRunner r(agg);
	r.runStage(RunStage::INIT);

	auto om = agg.getOne<OverrideHandler>();
	overrides_ = om->getPossibleOverrideIds();
	maintains_ = om->getPossibleMaintainIds();

	agg.cleanUp();
}
