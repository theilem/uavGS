//
// Created by mirco on 25.06.20.
//

#include "uavGS/SensorData/SensorDataManager.h"
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/SensorData/WidgetSensorData.h>
#include <uavGS/SensorData/QFlightInstruments/WidgetPFD.h>
#include <uavGS/SensorData/QFlightInstruments/WidgetSix.h>
#include <uavAP/Core/Frames/InertialFrame.h>
#include "uavGS/GSWidgetFactory.h"

bool
SensorDataManager::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSet<DataHandling, GSWidgetFactory>())
			{
				CPSLOG_ERROR << "SensorDataHandling missing deps";
				return true;
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<WidgetSensorData>();
			wf->registerWidget<WidgetPFD>();
			wf->registerWidget<WidgetSix>();
			break;
		}
		case RunStage::NORMAL:
		{
			auto dh = get<DataHandling>();
			dh->subscribeOnData<SensorData>(Content::SENSOR_DATA, [this](const auto& sd)
			{ onSensorData(sd); });
			dh->subscribeOnData<PowerData>(Content::POWER_DATA, [this](const auto& pd)
			{ onPowerData_(pd); });
			dh->subscribeOnData<ServoData>(Content::SERVO_DATA, [this](const auto& sd)
			{ onServoData_(sd); });
			dh->subscribeOnData<VehicleOneFrame>(Content::LOCAL_FRAME, [this](const auto& lf)
			{
				localFrame_ = lf;
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
