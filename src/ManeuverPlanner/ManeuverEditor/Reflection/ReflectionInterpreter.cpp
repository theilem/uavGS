//
// Created by seedship on 4/4/21.
//

#include <filesystem>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>
#include <cpsCore/Utilities/EnumMap.hpp>
#include <uavAP/Core/SensorData.h>
#include <uavAP/FlightAnalysis/Condition/Relational.h>
#include "uavGS/ManeuverPlanner/ManeuverEditor/Reflection/ReflectionInterpreter.h"

ReflectionInterpreter::ReflectionInterpreter()
{
	for (const auto& f: std::filesystem::directory_iterator(path))
	{
		// super entity, namely, condition or waveform
		const auto& superEntity = f.path();
		std::unordered_map<std::string, std::string> subEntities;
		if (f.is_directory())
		{
			std::string entityName = superEntity.c_str();
			std::filesystem::path mappingPath = f.path();
			Configuration mapping;
			boost::property_tree::read_json(mappingPath.append("mapping.json"), mapping);
			for (const auto& t : mapping)
			{
				subEntities[t.first] = t.second.get_value<std::string>();
			}
		}
		mappings_[superEntity.stem()] = subEntities;

		std::filesystem::path paramPath = superEntity;
		paramPath.append("params");
		for (const auto& paramFile: std::filesystem::directory_iterator(paramPath))
		{
			Configuration def;
			boost::property_tree::read_json(paramFile.path(), def);
			defs_[paramFile.path().filename()] = def;
		}
	}

	//NOTE: My original plan was to have this class be generic and reusable, but I cannot think of any way around this
	//Setting EnumDefs
	std::unordered_set<std::string> sensorEnumStrings;
	for (const auto & it : EnumMap<SensorEnum>::getInstance())
	{
		sensorEnumStrings.insert(it.second);
	}
	enumDefs_["SensorEnum"] = sensorEnumStrings;

	std::unordered_set<std::string> relationalStrings;
	for (const auto & it : EnumMap<Relational>::getInstance())
	{
		relationalStrings.insert(it.second);
	}
	enumDefs_["Relational"] = relationalStrings;

	//Verfiy proper creation
//	for (const auto& entityMap: mappings_)
//	{
//		for (const auto& mappingPair : entityMap.second)
//		{
//			std::cout << entityMap.first << ":"  << mappingPair.first << ":" << mappingPair.second <<"\n";
//		}
//	}
//	for (const auto& defPair: defs_)
//	{
//		std::cout << defPair.first << ":";
//		boost::property_tree::write_json(std::cout, defPair.second);
//	}
}

const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
ReflectionInterpreter::getMapping() const
{
	return mappings_;
}

const std::unordered_map<std::string, Configuration>&
ReflectionInterpreter::getConfiguration() const
{
	return defs_;
}

const std::unordered_map<std::string, std::unordered_set<std::string>>&
ReflectionInterpreter::getEnumDefs() const
{
	return enumDefs_;
}
