//
// Created by mirco on 25.06.20.
//

#include "uavGS/SensorData/SensorDataManager.h"
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/SensorData/WidgetSensorData.h>
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
			break;
		}
		case RunStage::NORMAL:
		{
			auto dh = get<DataHandling>();
			dh->subscribeOnData<SensorData>(Content::SENSOR_DATA, [this](const auto& sd)
			{ onSensorData(sd); });
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
