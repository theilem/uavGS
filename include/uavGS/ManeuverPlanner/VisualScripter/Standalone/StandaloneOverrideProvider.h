//
// Created by seedship on 4/9/21.
//

#ifndef UAVGS_STANDALONEOVERRIDEPROVIDER_H
#define UAVGS_STANDALONEOVERRIDEPROVIDER_H


#include <cpsCore/Aggregation/AggregatableObject.hpp>

#include <uavAP/Core/OverrideHandler/OverrideHandler.h>

#include "uavGS/ManeuverPlanner/VisualScripter/Standalone/IOverridesProvider.h"



class StandaloneOverrideProvider: public IOverridesProvider, public AggregatableObject<>
{
public:

	static constexpr TypeId typeId = "standalone_override_provider";

	void
	loadConfig(const std::string& path);

	const std::vector<std::string>&
	getOverrides() const override;

	const std::vector<std::string>&
	getMaintains() const override;



private:
	std::vector<std::string> overrides_;
	std::vector<std::string> maintains_;
};


#endif //UAVGS_STANDALONEOVERRIDEPROVIDER_H
