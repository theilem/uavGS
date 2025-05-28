//
// Created by mirco on 25.06.20.
//

#include "uavGS/SensorData/SensorDataManager.h"
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavAP/FlightControl/Controller/ControllerOutput.h>
#include <uavGS/SensorData/WidgetSensorData.h>
#include <uavGS/SensorData/QFI-Derived/WidgetPFD_GS.h>
#include <uavGS/SensorData/QFI-Derived/WidgetSix_GS.h>
#include <uavGS/SensorData/QFI-Derived/WidgetNAV_GS.h>
#include <uavAP/Core/Frames/InertialFrame.h>
#include <uavGS/SensorData/Recording/Recording.h>
#include <uavGS/Widgets/WidgetNetworkStats.h>
#include <uavGS/Widgets/NamedDataPlot.h>

#include "uavGS/GSWidgetFactory.h"

bool
SensorDataManager::run(RunStage stage)
{
    switch (stage)
    {
    case RunStage::INIT:
        {
            if (!checkIsSet<EnumBasedDataHandling, GSWidgetFactory>())
            {
                CPSLOG_ERROR << "SensorDataHandling missing deps";
                return true;
            }

            auto wf = get<GSWidgetFactory>();
            wf->registerWidget<WidgetSensorData>();
            wf->registerWidget<WidgetPFD_GS>();
            wf->registerWidget<WidgetSix_GS>();
            wf->registerWidget<WidgetNAV_GS>();
            wf->registerWidget<Recording>();
            wf->registerWidget<WidgetNetworkStats>();
            wf->registerWidget<NamedDataPlot>("controller_output_plot",
                [this](QWidget* parent)
                {
                    auto plot = new NamedDataPlot(parent);
                    plot->initialize<TimedValue<NamedControllerOutput>>();
                    subscribeOnControllerOutput([plot](const auto& co)
                    {
                        plot->onTimedData(co);
                    });
                    return plot;
                });
            wf->registerWidget<NamedDataPlot>("sensor_data_plot",
                [this](QWidget* parent)
                {
                    auto plot = new NamedDataPlot(parent);
                    plot->initialize<TimedValue<NamedSensorData>>();
                    subscribeOnTimedSensorData([plot](const auto& sd)
                    {
                        plot->onTimedData(sd);
                    });
                    return plot;
                });
            break;
        }
    case RunStage::NORMAL:
        {
            auto dh = get<EnumBasedDataHandling>();
            dh->subscribeOnData<SensorData>(Content::SENSOR_DATA, [this](const auto& sd)
            {
                onSensorData(sd);
            });
            dh->subscribeOnData<PowerData>(Content::POWER_DATA, [this](const auto& pd)
            {
                onPowerData_(pd);
            });
            dh->subscribeOnData<ServoData>(Content::SERVO_DATA, [this](const auto& sd)
            {
                onServoData_(sd);
            });
            dh->subscribeOnData<std::map<std::string, FloatingType>>(Content::MISC_VALUES, [this](const auto& mv)
            {
                onMiscValues_(mv);
            });
            dh->subscribeOnData<VehicleOneFrame>(Content::LOCAL_FRAME, [this](const auto& lf)
            {
                localFrame_ = lf;
            });
            dh->subscribeOnData<TimedValue<ControllerOutput>>(Content::CONTROLLER_OUTPUT, [this](const auto& co)
            {
                TimedValue<NamedControllerOutput> controllerOutput(co.first, NamedControllerOutput(co.second));
                onControllerOutput_(controllerOutput);
            });


            break;
        }
    default:
        break;
    }
    return false;
}

void
SensorDataManager::onSensorData(const SensorData& sd)
{
    sensorDataLocal_ = sd;
    sensorDataGlobal_ = sd;

    changeFrame(localFrame_, InertialFrame(), sensorDataGlobal_);

    onSensorDataLocal_(sensorDataLocal_);
    onSensorDataGlobal_(sensorDataGlobal_);

    auto timedSensorData = TimedValue<NamedSensorData>(sd.timestamp, NamedSensorData(sd));
    onTimedSensorData_(timedSensorData);
}

boost::signals2::connection
SensorDataManager::subscribeOnSensorDataLocal(const OnSensorData::slot_type& slot)
{
    return onSensorDataLocal_.connect(slot);
}

boost::signals2::connection
SensorDataManager::subscribeOnSensorDataGlobal(const OnSensorData::slot_type& slot)
{
    return onSensorDataGlobal_.connect(slot);
}


boost::signals2::connection
SensorDataManager::subscribeOnPowerData(const OnPowerData::slot_type& slot)
{
    return onPowerData_.connect(slot);
}

boost::signals2::connection
SensorDataManager::subscribeOnServoData(const OnServoData::slot_type& slot)
{
    return onServoData_.connect(slot);
}

boost::signals2::connection
SensorDataManager::subscribeOnMiscValues(const OnMiscValues::slot_type& slot)
{
    return onMiscValues_.connect(slot);
}

boost::signals2::connection
SensorDataManager::subscribeOnControllerOutput(const OnControllerOutput::slot_type& slot)
{
    return onControllerOutput_.connect(slot);
}

boost::signals2::connection
SensorDataManager::subscribeOnTimedSensorData(const OnTimedSensorData::slot_type& slot)
{
    return onTimedSensorData_.connect(slot);
}

const SensorData&
SensorDataManager::getSensorDataLocal()
{
    return sensorDataLocal_;
}
