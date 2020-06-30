//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WIDGETPARAMETERSETS_H
#define UAVGS_WIDGETPARAMETERSETS_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include "uavGS/ParameterSets/WidgetGeneric.h"
#include "uavGS/ParameterSets/WidgetPopulator.h"
#include "uavGS/ParameterSets/WidgetTreeParser.h"

template<class ParameterSet, Content content, Target target>
class WidgetParameterSets : public WidgetGeneric, public AggregatableObject<DataHandling>
{
public:

	WidgetParameterSets(QWidget* parent);

	void
	connect();

private:

	void
	updateHandle();

	void
	sendHandle();

	void
	requestHandle();

	ParameterSet params_;

};

template<class ParameterSet, Content content, Target target>
void
WidgetParameterSets<ParameterSet, content, target>::connect()
{
	if (auto dh = get<DataHandling>())
	{
		dh->subscribeOnData<ParameterSet>(content, [this](const ParameterSet& p)
		{
			this->params_ = p;
			this->contentUpdated();
		});
	}
}

template<class ParameterSet, Content content, Target target>
void
WidgetParameterSets<ParameterSet, content, target>::updateHandle()
{
	auto node = getNode();
	WidgetPopulator pop(node);

	params_.configure(pop);
}

template<class ParameterSet, Content content, Target target>
WidgetParameterSets<ParameterSet, content, target>::WidgetParameterSets(QWidget* parent):WidgetGeneric(parent)
{
	setHandles([this]
			   { this->sendHandle(); }, [this]
			   { this->requestHandle(); }, [this]
			   { this->updateHandle(); });
}

template<class ParameterSet, Content content, Target target>
void
WidgetParameterSets<ParameterSet, content, target>::requestHandle()
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(content, Content::REQUEST_CONFIG, target);
	}
}

template<class ParameterSet, Content content, Target target>
void
WidgetParameterSets<ParameterSet, content, target>::sendHandle()
{
	auto node = getNode();
	WidgetTreeParser pop(node);

	params_.configure(pop);

	if (auto dh = get<DataHandling>())
	{
		dh->sendData(params_, content, target);
	}
}


#endif //UAVGS_WIDGETPARAMETERSETS_H
