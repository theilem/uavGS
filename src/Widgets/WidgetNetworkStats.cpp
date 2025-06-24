//
// Created by Mirco Theile on 2/9/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WidgetNetworkStats.h" resolved


#include "uavGS/Widgets/WidgetNetworkStats.h"

#include <cpsCore/Utilities/IDC/NetworkLayer/INetworkLayer.h>
#include <cpsCore/Utilities/Scheduler/IScheduler.h>
#include <uavGS/ParameterSets/WidgetPopulator.h>

WidgetNetworkStats::WidgetNetworkStats(QWidget* parent):
    WidgetGeneric(parent)
{
    setHandles([]
               {  }, []
               {  }, [this]
               { this->updateStats(); });
}

WidgetNetworkStats::~WidgetNetworkStats()
{
    updateEvent_.cancel();
}

void
WidgetNetworkStats::connect()
{
    if (auto scheduler = get<IScheduler>())
    {
        updateEvent_ = scheduler->schedule([this]
        {
            this->contentUpdated();
        }, Milliseconds(0), Milliseconds(100));
    }
    else
    {
        CPSLOG_ERROR << "No scheduler available. Cannot display network stats.";
    }
}

void
WidgetNetworkStats::updateStats()
{
    auto network = get<INetworkLayer>();
    if (!network)
    {
        CPSLOG_ERROR << "No network layer available. Cannot display network stats. Canceling event.";
        updateEvent_.cancel();
        return;
    }

    auto stats = network->getStats();
    if (stats.empty())
    {
        CPSLOG_ERROR << "No network stats available. Canceling event.";
        updateEvent_.cancel();
        return;
    }

    if (stats.size() != 1)
    {
        CPSLOG_WARN << "More than one network stats available. Not implemented. Only displaying first.";
    }
    auto name = stats.begin()->first;
    auto stat = stats.begin()->second;

    setTitle(name);
    WidgetPopulator<false> pop(getNode());
    stat.parse(pop);
}
