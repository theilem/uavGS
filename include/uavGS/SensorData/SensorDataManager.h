//
// Created by mirco on 25.06.20.
//

#ifndef UAVGS_SENSORDATAMANAGER_H
#define UAVGS_SENSORDATAMANAGER_H

#include <cpsCore/cps_object>
#include <uavAP/Core/SensorData.h>
#include <uavAP/Core/Frames/VehicleOneFrame.h>

#include <boost/signals2.hpp>
#include <uavAP/FlightControl/Controller/ControllerOutput.h>

class DataHandling;
class GSWidgetFactory;

class SensorDataManager : public AggregatableObject<DataHandling, GSWidgetFactory>, public IRunnableObject
{
public:
    static constexpr TypeId typeId = "sensor_data";

    SensorDataManager() = default;

    bool
    run(RunStage stage) override;

    using OnSensorData = boost::signals2::signal<void(const SensorData&)>;
    using OnPowerData = boost::signals2::signal<void(const PowerData&)>;
    using OnServoData = boost::signals2::signal<void(const ServoData&)>;
    using OnMiscValues = boost::signals2::signal<void(const std::map<std::string, FloatingType>&)>;
    using OnControllerOutput = boost::signals2::signal<void(const TimedValue<NamedControllerOutput>&)>;
    using OnTimedSensorData = boost::signals2::signal<void(const TimedValue<NamedSensorData>&)>;

    boost::signals2::connection
    subscribeOnSensorDataLocal(const OnSensorData::slot_type& slot);

    boost::signals2::connection
    subscribeOnSensorDataGlobal(const OnSensorData::slot_type& slot);

    boost::signals2::connection
    subscribeOnPowerData(const OnPowerData::slot_type& slot);

    boost::signals2::connection
    subscribeOnServoData(const OnServoData::slot_type& slot);

    boost::signals2::connection
    subscribeOnMiscValues(const OnMiscValues::slot_type& slot);

    boost::signals2::connection
    subscribeOnControllerOutput(const OnControllerOutput::slot_type& slot);

    boost::signals2::connection
    subscribeOnTimedSensorData(const OnTimedSensorData::slot_type& slot);

    const SensorData&
    getSensorDataLocal();

private:
    void
    onSensorData(const SensorData& sd);

    SensorData sensorDataLocal_;
    SensorData sensorDataGlobal_;
    VehicleOneFrame localFrame_;

    OnSensorData onSensorDataLocal_;
    OnSensorData onSensorDataGlobal_;
    OnPowerData onPowerData_;
    OnServoData onServoData_;
    OnMiscValues onMiscValues_;
    OnControllerOutput onControllerOutput_;
    OnTimedSensorData onTimedSensorData_;
};


#endif //UAVGS_SENSORDATAMANAGER_H
