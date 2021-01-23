//
// Created by mirco on 25.06.20.
//

#include "uavGS/SensorData/SensorDataManager.h"
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/SensorData/WidgetSensorData.h>
#include <uavGS/SensorData/QFI-Derived/WidgetPFD_GS.h>
#include <uavGS/SensorData/QFI-Derived/WidgetSix_GS.h>
#include <uavGS/SensorData/QFI-Derived/WidgetNAV_GS.h>
#include <uavAP/Core/Frames/InertialFrame.h>
#include <uavGS/SensorData/Recording/Recording.h>
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
			wf->registerWidget<WidgetPFD_GS>();
			wf->registerWidget<WidgetSix_GS>();
			wf->registerWidget<WidgetNAV_GS>();
			wf->registerWidget<Recording>();
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
			dh->subscribeOnData<std::map<std::string, FloatingType>>(Content::MISC_VALUES, [this](const auto& mv)
			{ onMiscValues_(mv); });
			dh->subscribeOnData<LocalFrame>(Content::LOCAL_FRAME, [this](const auto& lf)
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

	localFrame_.toGlobalFrame(sensorDataGlobal_);

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

boost::signals2::connection
SensorDataManager::subscribeOnMiscValues(const OnMiscValues::slot_type& slot)
{
	return onMiscValues_.connect(slot);
}

const SensorData&
SensorDataManager::getSensorDataLocal()
{
	return sensorDataLocal_;
}

