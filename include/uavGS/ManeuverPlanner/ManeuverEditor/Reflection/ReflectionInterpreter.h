//
// Created by seedship on 4/4/21.
//

#ifndef UAVGS_REFLECTIONINTERPRETER_H
#define UAVGS_REFLECTIONINTERPRETER_H


#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cpsCore/Configuration/Configuration.hpp>
#include <cpsCore/Aggregation/AggregatableObject.hpp>

class ReflectionInterpreter: public AggregatableObject<>
{
public:

	static constexpr TypeId typeId = "reflection_interpreter";

	// We could consider making this configurable
	static constexpr auto path = "/usr/local/resources/uavGS/ReflectionData";

	ReflectionInterpreter();

	const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
	getMapping() const;

	const std::unordered_map<std::string, Configuration>&
	getConfiguration() const;

	const std::unordered_map<std::string, std::unordered_set<std::string>>&
	getEnumDefs() const;

private:
	//ex: Condition-> ["duration"->DurationConditionParams.json]
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mappings_;
	std::unordered_map<std::string, Configuration> defs_;

	std::unordered_map<std::string, std::unordered_set<std::string>> enumDefs_;
};


#endif //UAVGS_REFLECTIONINTERPRETER_H
