//
// Created by mirco on 19.06.20.
//

#include "uavGS/PID/PIDConfigurator.h"
#include "uavGS/GSWidgetFactory.h"
#include "uavGS/ParameterSets/WidgetParameterSets.h"
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavAP/FlightControl/ThrottleLimiter/ThrottleLimiterParams.h>
#include <uavGS/PID/Widgets/WidgetCPGrid.h>
#include <uavGS/PID/Widgets/WidgetPIDPlots.h>

bool
PIDConfigurator::tunePID(const PIDTuning& tunePID)
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(tunePID, Content::TUNE_PID, Target::FLIGHT_CONTROL);
		return true;
	}
	return false;
}

const PIDParams&
PIDConfigurator::getPIDMap() const
{
	return pidParams_;
}

bool
PIDConfigurator::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSet<DataHandling, GSWidgetFactory>())
			{
				CPSLOG_ERROR << "PIDConfigurator: Missing dependencies";
				return true;
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<WidgetCPGrid>();
			wf->registerWidget<WidgetPIDPlots>();
			wf->registerWidget<WidgetParameterSets<ThrottleLimiterParams,
					Content::THROTTLE_LIMITER_PARAMS,
					Target::FLIGHT_CONTROL>>("throttle_limit");
			break;
		}
		case RunStage::NORMAL:
		{
			auto dh = get<DataHandling>();
			dh->subscribeOnData<PIDParams>(Content::PID_PARAMS, [this](const PIDParams& p)
			{ this->pidParams_ = p;});
		}
		default:
			break;
	}
	return false;
}

void
PIDConfigurator::requestPIDParams()
{
	auto dh = get<DataHandling>();
	dh->sendData(DataRequest::PID_PARAMS, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
}
