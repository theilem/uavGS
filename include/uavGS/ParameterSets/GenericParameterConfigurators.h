//
// Created by seedship on 4/9/21.
//

#ifndef UAVGS_GENERICPARAMETERCONFIGURATORS_H
#define UAVGS_GENERICPARAMETERCONFIGURATORS_H


#include <cpsCore/Synchronization/IRunnableObject.h>

class GSWidgetFactory;

class GenericParameterConfigurators: public AggregatableObject<GSWidgetFactory>, public IRunnableObject
{
public:
	static constexpr TypeId typeId = "generic_params";

	bool
	run(RunStage stage) override;
};


#endif //UAVGS_GENERICPARAMETERCONFIGURATORS_H
