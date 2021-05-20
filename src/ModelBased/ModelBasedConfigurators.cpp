//
// Created by seedship on 5/17/21.
//

#include <uavGS/ModelBased/ModelBasedHelperWidget.h>
#include "uavGS/ModelBased/ModelBasedConfigurators.h"
#include "uavGS/GSWidgetFactory.h"

bool
ModelBasedConfigurators::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSet<GSWidgetFactory>())
			{
				CPSLOG_ERROR << "Missing dependencies";
				return true;
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<ModelBasedHelperWidget>("model_based_helper");
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
