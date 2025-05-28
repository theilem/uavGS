/**
 *   @author Richard Nai, rnai2@illinois.edu
 *   @file IDataSignals.h
 *   @date [dd/mm/yyyy] 3/21/2018
 *   @brief UAV Ground Station PID Configuration Interface
 */

#ifndef IPIDCONFIGURATOR_H
#define IPIDCONFIGURATOR_H

#include <boost/signals2.hpp>

#include <cpsCore/cps_object>

#include "uavAP/FlightControl/Controller/PIDController/PIDHandling.h"
#include "uavAP/Core/DataHandling/Content.hpp"

class IScheduler;
template <typename C, typename T>
class DataHandling;
class GSWidgetFactory;

/**
 * @brief   The IPIDConfigurator class serves as an interface for all classes
 *          that have PID tuning functionality.
 */
class PIDConfigurator : public AggregatableObject<DataHandling<Content, Target>, GSWidgetFactory, IScheduler>, public IRunnableObject
{
public:
    static constexpr auto typeId = "pid_configurator";

    enum class PIDSyncStatus
    {
        UNKNOWN = 0,
        MATCHING,
        NOT_MATCHING,
    };

    bool
    run(RunStage stage) override;

    void
    applyPID(PIDs pid);

    void
    localChanged(PIDs pid);

    void
    requestPIDParams();

    void
    requestSinglePIDParams(PIDs pid);

    Control::PIDParameters*
    getPIDParams(PIDs pid);

    PIDSyncStatus*
    getSyncStatus(PIDs pid);

    std::vector<PIDs>
    getPIDs() const;

    void
    savePIDConfig(const std::string& filename);

    using onLocalPIDsCleared = boost::signals2::signal<void()>;
    using onLocalPIDsSet = boost::signals2::signal<void(PIDParams&, std::map<PIDs, PIDSyncStatus>&)>;
    using onSyncUpdated = boost::signals2::signal<void()>;

    boost::signals2::connection
    subscribeOnLocalPIDsCleared(const onLocalPIDsCleared::slot_type& subscriber);

    boost::signals2::connection
    subscribeOnLocalPIDsSet(const onLocalPIDsSet::slot_type& subscriber, bool callImmediately = false);

    boost::signals2::connection
    subscribeOnSyncUpdated(const onSyncUpdated::slot_type& subscriber);

private:
    void
    checkSync(PIDs pid);

    PIDParams localPIDParams_;
    PIDParams remotePIDParams_;
    std::map<PIDs, PIDSyncStatus> syncStatus_;

    //Subscribers
    onLocalPIDsCleared onLocalPIDsCleared_;
    onLocalPIDsSet onLocalPIDsSet_;
    onSyncUpdated onSyncUpdated_;
};

#endif // IPIDCONFIGURATOR_H
