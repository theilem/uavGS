/**
 * @author Mirco Theile, mircot@illinois.edu
 * @file GSWidgetFactory.h
 * @date Jan 9, 2018
 * @brief
 */

#ifndef GROUND_STATION_INCLUDE_GROUND_STATION_GSWIDGETFACTORY_H_
#define GROUND_STATION_INCLUDE_GROUND_STATION_GSWIDGETFACTORY_H_

#include <map>
#include <memory>
#include <QWidget>
#include <type_traits>

#include <cpsCore/Aggregation/AggregatableObjectMaster.hpp>
#include <functional>
#include "uavGS/GroundStationHelper.h"

class GSWidgetFactory : public AggregatableObjectMaster<>
{
public:

	static constexpr TypeId typeId = "widget_factory";

	GSWidgetFactory() = default;

	QWidget*
	createWidget(const std::string& type, QWidget* parent = nullptr);

	template<typename TYPE>
	TYPE*
	createWidget(QWidget* parent = nullptr);

	template<typename TYPE>
	void
	connectWidget(TYPE* widget);

	std::vector<std::string>
	getWidgetTypes();

	template<typename TYPE>
	typename std::enable_if<std::is_base_of<QWidget, TYPE>::value, void>::type
	registerWidget();

	template<typename TYPE>
	typename std::enable_if<std::is_base_of<QWidget, TYPE>::value, void>::type
	registerWidget(const std::string& type);

private:

	using WidgetCreator = std::function<QWidget*(QWidget*)>;

	std::map<std::string, WidgetCreator> creators_;

};

template<typename TYPE>
inline typename std::enable_if<std::is_base_of<QWidget, TYPE>::value, void>::type
GSWidgetFactory::registerWidget()
{
	const std::string type(TYPE::widgetName);
	registerWidget<TYPE>(type);
}

template<typename TYPE>
inline typename std::enable_if<std::is_base_of<QWidget, TYPE>::value, void>::type
GSWidgetFactory::registerWidget(const std::string& type)
{
	CPSLOG_DEBUG << "Added widget " << type;
	if (creators_.find(type) != creators_.end())
	{
		CPSLOG_ERROR << "Widget of that type already exists. Ignore new insertion.";
		return;
	}
	if constexpr (std::is_base_of<IAggregatableObject, TYPE>::value)
		creators_.insert(std::make_pair(type, [this](QWidget* parent){return this->createWidget<TYPE>(parent);}));
	else
		creators_.insert(std::make_pair(type, &TYPE::createGSWidget));
}

template<class TYPE>
TYPE*
GSWidgetFactory::createWidget(QWidget* parent)
{
	auto widget(new TYPE(parent));
	widget->notifyAggregationOnUpdate(*this->aggregator_);
	widget->connect();
	return widget;
}

template<class TYPE>
void
GSWidgetFactory::connectWidget(TYPE *widget)
{
	widget->notifyAggregationOnUpdate(*this->aggregator_);
	widget->connect();
}

#endif /* GROUND_STATION_INCLUDE_GROUND_STATION_GSWIDGETFACTORY_H_ */
