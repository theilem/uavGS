//
// Created by mirco on 29.06.20.
//

#include "uavGS/ManeuverPlanner/PlanningManager.h"
#include "uavGS/GSWidgetFactory.h"
#include <uavGS/ManeuverPlanner/WidgetManeuverPlanner.h>
#include <uavAP/Core/DataHandling/DataHandling.h>

bool
PlanningManager::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSet<DataHandling, GSWidgetFactory>())
			{
				CPSLOG_ERROR << "Missing dependencies";
				return true;
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<WidgetManeuverPlanner>();
			break;
		}
		case RunStage::NORMAL:
		{
			break;
		}
		default:
			break;
	}
	return false;
}
