//
// Created by mirco on 23.06.25.
//

#ifndef WIDGETNAMEDDATA_HPP
#define WIDGETNAMEDDATA_HPP
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include "uavGS/ParameterSets/WidgetGeneric.h"

#include <QWidget>

#include "uavGS/ParameterSets/WidgetPopulator.h"
#include "uavGS/ParameterSets/WidgetTreeParser.h"

template <class NamedDataStruct, class Manager>
class WidgetNamedData : public WidgetGeneric, public AggregatableObject<Manager>
{
public:
	static constexpr auto name = NamedDataStruct::name;

	explicit WidgetNamedData(QWidget* parent);

	void
	connect();

private:
	void
	updateHandle();

	void
	sendHandle();

	void
	requestHandle();

	NamedDataStruct data_;
};

template <class NamedDataStruct, class Manager>
WidgetNamedData<NamedDataStruct, Manager>::WidgetNamedData(QWidget* parent): WidgetGeneric(parent)
{
	setHandles([this]
	{
		this->sendHandle();
	}, [this]
	{
		this->requestHandle();
	}, [this]
	{
		this->updateHandle();
	});
	std::stringstream ss;
	ss << Manager::typeId << "->" << name;
	setTitle(ss.str().c_str());
}


template <class NamedDataStruct, class Manager>
void
WidgetNamedData<NamedDataStruct, Manager>::connect()
{
	auto man = this->template get<Manager>();
	if (!man)
	{
		CPSLOG_ERROR << "Manager not in aggregation, cannot connect";
		return;
	}
	data_ = man->template getNamedData<NamedDataStruct>();
	contentUpdated();
}

template <class NamedDataStruct, class Manager>
void
WidgetNamedData<NamedDataStruct, Manager>::updateHandle()
{
	auto node = getNode();
	WidgetPopulator<true> pop(node);
	data_.configure(pop);
}
template <class NamedDataStruct, class Manager>
void
WidgetNamedData<NamedDataStruct, Manager>::requestHandle()
{
	auto man = this->template get<Manager>();
	if (!man)
	{
		CPSLOG_ERROR << "Manager not in aggregation, cannot connect";
		return;
	}
	data_ = man->template getNamedData<NamedDataStruct>();
	updateHandle();
}

template <class NamedDataStruct, class Manager>
void
WidgetNamedData<NamedDataStruct, Manager>::sendHandle()
{
	auto node = getNode();
	WidgetTreeParser pop(node);

	data_.configure(pop);

	if (auto man = this->template get<Manager>())
	{
		man->template setNamedData<NamedDataStruct>(data_);
	}
}
#endif //WIDGETNAMEDDATA_HPP
