//
// Created by seedship on 5/17/21.
//

#ifndef UAVGS_MODELBASEDCONFIGURATORS_H
#define UAVGS_MODELBASEDCONFIGURATORS_H


#include <cpsCore/Synchronization/IRunnableObject.h>
#include <cpsCore/Aggregation/AggregatableObject.hpp>

class GSWidgetFactory;

class ModelBasedConfigurators: public AggregatableObject<GSWidgetFactory>, public IRunnableObject
{
public:
	static constexpr TypeId typeId = "model_based";

	bool
	run(RunStage stage) override;

};


#endif //UAVGS_MODELBASEDCONFIGURATORS_H
