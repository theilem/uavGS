//
// Created by mirco on 06.07.20.
//

#ifndef UAVGS_WIDGETCONFIGURABLEOBJECT_H
#define UAVGS_WIDGETCONFIGURABLEOBJECT_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include "uavGS/ParameterSets/WidgetGeneric.h"
#include "uavGS/ParameterSets/WidgetPopulator.h"
#include "uavGS/ParameterSets/WidgetTreeParser.h"

template<class ConfigurableObject>
class WidgetConfigurableObject : public WidgetGeneric, public AggregatableObject<ConfigurableObject>
{
public:

	WidgetConfigurableObject(QWidget* parent);

	void
	connect();

private:

	void
	updateHandle();

	void
	sendHandle();

	void
	requestHandle();

};

template<class ConfigurableObject>
void
WidgetConfigurableObject<ConfigurableObject>::connect()
{
	if (!this->template isSet<ConfigurableObject>())
	{
		CPSLOG_ERROR << "Configurable Object not in aggregation, cannot connect";
	}
}

template<class ConfigurableObject>
void
WidgetConfigurableObject<ConfigurableObject>::updateHandle()
{
	if (auto co = this->template get<ConfigurableObject>())
	{
		auto node = getNode();
		WidgetPopulator<true> pop(node);

		co->getParams().configure(pop);
	}
}

template<class ConfigurableObject>
WidgetConfigurableObject<ConfigurableObject>::WidgetConfigurableObject(QWidget* parent):WidgetGeneric(parent)
{
	setHandles([this]
			   { this->sendHandle(); }, [this]
			   { this->requestHandle(); }, [this]
			   { this->updateHandle(); });
	setTitle(typeid(ConfigurableObject()).name());
}

template<class ConfigurableObject>
void
WidgetConfigurableObject<ConfigurableObject>::requestHandle()
{
	this->contentUpdated();
}

template<class ConfigurableObject>
void
WidgetConfigurableObject<ConfigurableObject>::sendHandle()
{
	if (auto co = this->template get<ConfigurableObject>())
	{
		auto node = getNode();
		WidgetTreeParser pop(node);
		typename ConfigurableObject::ParamType params;
		params.configure(pop);

		co->setParams(params);
	}
}

#endif //UAVGS_WIDGETCONFIGURABLEOBJECT_H
