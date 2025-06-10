//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WidgetParameterStruct_H
#define UAVGS_WidgetParameterStruct_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include "uavGS/ParameterSets/WidgetGeneric.h"
#include "uavGS/ParameterSets/WidgetPopulator.h"
#include "uavGS/ParameterSets/WidgetTreeParser.h"

template <class ParameterSet, class Manager>
class WidgetParameterStruct : public WidgetGeneric, public AggregatableObject<Manager>
{
public:
    explicit WidgetParameterStruct(QWidget* parent);

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

template <class ParameterSet, class Manager>
void
WidgetParameterStruct<ParameterSet, Manager>::connect()
{
    auto man = this->template get<Manager>();
    if (!man)
    {
        CPSLOG_ERROR << "Manager not in aggregation, cannot connect";
        return;
    }
    params_ = man->getWidgetData();
    contentUpdated();

}

template <class ParameterSet, class Manager>
void
WidgetParameterStruct<ParameterSet, Manager>::updateHandle()
{
    auto node = getNode();
    WidgetPopulator pop(node);

    params_.configure(pop);
}

template <class ParameterSet, class Manager>
WidgetParameterStruct<ParameterSet, Manager>::WidgetParameterStruct(QWidget* parent): WidgetGeneric(parent)
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
    setTitle(typeid(ParameterSet()).name());
}

template <class ParameterSet, class Manager>
void
WidgetParameterStruct<ParameterSet, Manager>::requestHandle()
{
    CPSLOG_DEBUG << "Not implemented";
}

template <class ParameterSet, class Manager>
void
WidgetParameterStruct<ParameterSet, Manager>::sendHandle()
{
    auto node = getNode();
    WidgetTreeParser pop(node);

    params_.configure(pop);

    if (auto man = this->template get<Manager>())
    {
        man->setWidgetData(params_);
    }
}


#endif //UAVGS_WidgetParameterStruct_H
