//
// Created by Mirco Theile on 2/9/24.
//

#ifndef WIDGETNETWORKSTATS_H
#define WIDGETNETWORKSTATS_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <cpsCore/Utilities/Scheduler/Event.h>

#include "uavGS/ParameterSets/WidgetGeneric.h"

class INetworkLayer;
class IScheduler;


class WidgetNetworkStats : public WidgetGeneric,
                           public AggregatableObject<INetworkLayer, IScheduler>
{
public:
    explicit WidgetNetworkStats(QWidget* parent = nullptr);

    ~WidgetNetworkStats();

    void
    connect();

private:

    void
    updateStats();

    Event updateEvent_;


};


#endif //WIDGETNETWORKSTATS_H
