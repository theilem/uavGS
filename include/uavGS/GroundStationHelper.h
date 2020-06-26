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
#include <uavAP/Core/DataHandling/DataHandling.h>

//#include <uavGS/RadioComm/RadioComm.h>
#include <uavGS/GSWidgetFactory.h>
#include <uavGS/PID/PIDConfigurator.h>
#include <uavGS/MapLogic/MapLogic.h>
#include <uavGS/LayoutGenerator/LayoutGenerator.h>
#include <uavGS/SensorData/SensorDataManager.h>

#include <cpsCore/Framework/StaticHelper.h>

using GroundStationHelper = StaticHelper<SchedulerFactory,
		TimeProviderFactory,
		IPC,
		IDC,
		NetworkFactory,
		DataPresentation,
		SignalHandler,
		DataHandling,
		GSWidgetFactory,
		PIDConfigurator,
		MapLogic,
		LayoutGenerator,
		SensorDataManager
		>;


#endif //UAVGS_GROUNDSTATIONHELPER_H
