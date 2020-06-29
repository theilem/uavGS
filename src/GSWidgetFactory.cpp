/*
 * GSWidgetFactory.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: mircot
 */

#include <uavGS/GSWidgetFactory.h>
#include <uavGS/PID/Widgets/WidgetCPGrid.h>
#include <uavGS/PID/Widgets/WidgetPIDPlots.h>

QWidget*
GSWidgetFactory::createWidget(const std::string& type, QWidget* parent)
{
	auto it = creators_.find(type);
	if (it == creators_.end())
	{
		CPSLOG_ERROR << "Widget type not found.";
		return nullptr;
	}
	return it->second(parent);
}

std::vector<std::string>
GSWidgetFactory::getWidgetTypes()
{
	std::vector<std::string> vec;
	for (auto& it : creators_)
		vec.push_back(it.first);
	return vec;
}
