//
// Created by mirco on 19.06.20.
//

#include "uavGS/PID/PIDConfigurator.h"

#include <cpsCore/Configuration/JsonPopulator.h>

#include "uavGS/GSWidgetFactory.h"
#include "uavGS/ParameterSets/WidgetParameterSets.h"
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavAP/FlightControl/ThrottleLimiter/ThrottleLimiterParams.h>
#include <uavGS/PID/Widgets/WidgetCPGrid.h>
// #include <uavGS/PID/Widgets/WidgetPIDPlots.h>


bool
PIDConfigurator::run(RunStage stage)
{
    switch (stage)
    {
    case RunStage::INIT:
        {
            if (!checkIsSetAll())
            {
                CPSLOG_ERROR << "PIDConfigurator: Missing dependencies";
                return true;
            }

            auto wf = get<GSWidgetFactory>();
            wf->registerWidget<WidgetCPGrid>();
            // wf->registerWidget<WidgetPIDPlots>();
            wf->registerWidget<WidgetParameterSets<ThrottleLimiterParams,
                                                   Content::THROTTLE_LIMITER_PARAMS,
                                                   Target::FLIGHT_CONTROL>>("throttle_limit");
            break;
        }
    case RunStage::NORMAL:
        {
            auto dh = get<DataHandling<Content, Target>>();
            dh->subscribeOnData<PIDParams>(Content::PID_PARAMS, [this](const PIDParams& p)
            {
                onLocalPIDsCleared_();
                remotePIDParams_ = p;
                localPIDParams_ = p;
                syncStatus_.clear();
                for (const auto& it : p)
                {
                    syncStatus_[it.first] = PIDSyncStatus::MATCHING;
                }
                onLocalPIDsSet_(localPIDParams_, syncStatus_);
            });

            dh->subscribeOnData<PIDTuning>(Content::SINGLE_PID_PARAMS, [this](const PIDTuning& p)
            {
                auto it = remotePIDParams_.find(p.pid);
                if (it == remotePIDParams_.end())
                {
                    CPSLOG_ERROR <<
                        "PIDConfigurator: Received PID parameters for unknown PID. Requesting all PID parameters.";
                    requestPIDParams();
                    return;
                }
                it->second = p.params;
                checkSync(p.pid);
            });

            auto scheduler = get<IScheduler>();
            scheduler->schedule([this]()
            {
                this->requestPIDParams();
            }, Seconds(1));
        }
    default:
        break;
    }
    return false;
}

void
PIDConfigurator::applyPID(PIDs pid)
{
    auto dh = get<DataHandling<Content, Target>>();
    dh->sendData(PIDTuning{pid, localPIDParams_[pid]}, Content::TUNE_PID, Target::FLIGHT_CONTROL);
    if (syncStatus_[pid] == PIDSyncStatus::NOT_MATCHING)
        syncStatus_[pid] = PIDSyncStatus::UNKNOWN;

    auto scheduler = get<IScheduler>();
    scheduler->schedule([this, pid]()
    {
        this->requestSinglePIDParams(pid);
    }, Seconds(1));
    onSyncUpdated_();
}

void
PIDConfigurator::checkSync(PIDs pid)
{
    auto syncStatus = syncStatus_.find(pid);
    auto localParams = localPIDParams_.find(pid);
    auto remoteParams = remotePIDParams_.find(pid);
    if (syncStatus == syncStatus_.end() ||
        localParams == localPIDParams_.end() ||
        remoteParams == remotePIDParams_.end())
    {
        CPSLOG_ERROR << "Cannot check sync status. PIDs not synced! Requesting all PID parameters.";
        requestPIDParams();
        return;
    }

    if (localParams->second == remoteParams->second)
    {
        syncStatus->second = PIDSyncStatus::MATCHING;
    }
    else
    {
        syncStatus->second = PIDSyncStatus::NOT_MATCHING;
    }
    onSyncUpdated_();
}

void
PIDConfigurator::localChanged(PIDs pid)
{
    auto syncStatus = syncStatus_.find(pid);
    if (syncStatus == syncStatus_.end())
        return;
    if (syncStatus->second == PIDSyncStatus::UNKNOWN)
        return;

    checkSync(pid);
}

void
PIDConfigurator::requestSinglePIDParams(PIDs pid)
{
    auto dh = get<DataHandling<Content, Target>>();
    dh->sendData(pid, Content::REQUEST_SINGLE_PID_PARAMS, Target::FLIGHT_CONTROL);
}

Control::PIDParameters*
PIDConfigurator::getPIDParams(PIDs pid)
{
    auto it = localPIDParams_.find(pid);
    if (it == localPIDParams_.end())
        return nullptr;
    return &it->second;
}

PIDConfigurator::PIDSyncStatus*
PIDConfigurator::getSyncStatus(PIDs pid)
{
    auto it = syncStatus_.find(pid);
    if (it == syncStatus_.end())
        return nullptr;
    return &it->second;
}

std::vector<PIDs>
PIDConfigurator::getPIDs() const
{
    std::vector<PIDs> pidIDs;
    for (const auto& it : remotePIDParams_)
    {
        pidIDs.push_back(it.first);
    }
    return pidIDs;
}

void
PIDConfigurator::savePIDConfig(const std::string& filename)
{
    Configuration config;
    for (const auto& [key, value] : localPIDParams_)
    {
        JsonPopulator pop;
        pop & value;
        config[EnumMap<PIDs>::convert(key)] = pop.getConfig();
    }
    std::fstream file(filename, std::ios::out);
    if (!file.is_open())
    {
        CPSLOG_ERROR << "Could not open file " << filename;
        return;
    }
    file << config.dump(4);
}

boost::signals2::connection
PIDConfigurator::subscribeOnLocalPIDsCleared(const onLocalPIDsCleared::slot_type& subscriber)
{
    return onLocalPIDsCleared_.connect(subscriber);
}

boost::signals2::connection
PIDConfigurator::subscribeOnLocalPIDsSet(const onLocalPIDsSet::slot_type& subscriber, bool callImmediately)
{
    if (!localPIDParams_.empty() && callImmediately)
        subscriber(localPIDParams_, syncStatus_);
    return onLocalPIDsSet_.connect(subscriber);
}

boost::signals2::connection
PIDConfigurator::subscribeOnSyncUpdated(const onSyncUpdated::slot_type& subscriber)
{
    return onSyncUpdated_.connect(subscriber);
}

void
PIDConfigurator::requestPIDParams()
{
    auto dh = get<DataHandling<Content, Target>>();
    dh->sendData(DataRequest::PID_PARAMS, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
}
