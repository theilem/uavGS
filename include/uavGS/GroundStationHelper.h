//
// Created by mirco on 17.06.20.
//

#ifndef UAVGS_GROUNDSTATIONHELPER_H
#define UAVGS_GROUNDSTATIONHELPER_H


#include <cpsCore/Utilities/Scheduler/SchedulerFactory.h>
#include <cpsCore/Utilities/TimeProvider/TimeProviderFactory.h>
#include <cpsCore/Utilities/DataPresentation/DataPresentation.h>
#include <cpsCore/Utilities/IPC/IPC.h>
#include <cpsCore/Utilities/IDC/IDC.h>
#include <cpsCore/Utilities/IDC/NetworkLayer/NetworkFactory.h>
#include <cpsCore/Utilities/IDC/TransportLayer/ITransportLayer.h>
#include <cpsCore/Utilities/SignalHandler/SignalHandler.h>

//#include <uavGS/RadioComm/RadioComm.h>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/GSWidgetFactory.h>
#include <uavGS/PID/PIDConfigurator.h>
#include <uavGS/MapLogic/MapLogic.h>
#include <uavGS/LayoutGenerator/LayoutGenerator.h>
#include <uavGS/SensorData/SensorDataManager.h>
#include <uavGS/ManeuverPlanner/PlanningManager.h>
#include <uavGS/ParameterSets/GenericParameterConfigurators.h>
#include <uavGS/ModelBased/ModelBasedConfigurators.h>
#include <uavGS/PacketForwarding/PacketForwarding.h>

#include <cpsCore/Framework/StaticHelper.h>

using GroundStationDefaults = StaticHelper<
		IPC,
		IDC,
		SchedulerFactory,
		TimeProviderFactory,
		SignalHandler,
		DataPresentation,
		GenericParameterConfigurators,
		ModelBasedConfigurators
>;

using GroundStationHelper = StaticHelper<GroundStationDefaults,
		NetworkFactory,
		DataHandling,
		GSWidgetFactory,
		PIDConfigurator,
		MapLogic,
		LayoutGenerator,
		SensorDataManager,
		PlanningManager,
		PacketForwarding
>;


#endif //UAVGS_GROUNDSTATIONHELPER_H
