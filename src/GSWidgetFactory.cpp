/*
 * GSWidgetFactory.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: mircot
 */

#include <uavGS/GSWidgetFactory.h>
#include <uavGS/PID/Widgets/WidgetCPGrid.h>
#include <uavGS/PID/Widgets/WidgetPIDPlots.h>

//GSWidgetFactory::GSWidgetFactory()
//{
//	//Widgets creatable by the factory need to have a:
//	//static const char widgetName[] = "enter_widget_name_here"
//	addWidget<WidgetCPGrid>();
////	addWidget<WidgetManeuverPlanner>();
////	addWidget<WidgetSteadyStateAnalysis>();
////	addWidget<WidgetOverheadMap>();
////	addWidget<WidgetPFD>();
//	addWidget<WidgetPIDPlots>();
////	addWidget<WidgetSensorData>();
////	addWidget<WidgetSix>();
////	addWidget<WidgetAdvancedControl>();
////	addWidget<WidgetLocalFrame>();
////	addWidget<WidgetTrimAnalysis>();
////	addWidget<WidgetManualWind>();
//}

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
