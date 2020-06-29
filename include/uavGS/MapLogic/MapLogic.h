#ifndef MAPLOGIC_H
#define MAPLOGIC_H

#include <uavAP/MissionControl/GlobalPlanner/Trajectory.h>
#include <uavAP/MissionControl/MissionPlanner/Mission.h>
#include <uavAP/FlightControl/LocalPlanner/LinearLocalPlanner/LinearLocalPlanner.h>
#include <uavAP/Core/Frames/VehicleOneFrame.h>
#include <uavAP/MissionControl/Geofencing/Rectanguloid.h>

#include "MapLocation.h"
#include "MapLogicParams.h"
#include <cpsCore/cps_object>
#include <uavAP/Core/SensorData.h>
#include <boost/signals2/signal.hpp>

class IScheduler;
class DataHandling;
class IScheduler;
class LayoutGenerator;
class GSWidgetFactory;

class MapLogic: public AggregatableObject<DataHandling, IScheduler, GSWidgetFactory, LayoutGenerator>, public IRunnableObject,
		public ConfigurableObject<MapLogicParams>
{
public:

	static constexpr TypeId typeId = "map_logic";

	MapLogic();

	bool
	run(RunStage stage) override;

	const std::vector<Waypoint>&
	getWaypoints() const;

	const Trajectory&
	getPath() const;

	int
	getCurrentPathSection() const;

	void
	askForAll();

	void
	askForMission();

	void
	askForTrajectory();

	void
	askForSafetyNet();

	void
	askForLocalFrame();

	void
	setSafetyBounds(const Rectanguloid& rect);

	void
	setLocalFrame(const VehicleOneFrame& frame);

	const Rectanguloid&
	getSafetyBounds() const;

	const SensorData&
	getSensorData() const;

	std::string
	getMapTileDirectory() const;

	std::string
	getIconPath() const;

	boost::signals2::connection
	subscribeOnUpdates(const boost::signals2::signal<void(void)>::slot_type& slot);

	MapLocation
	getMapCenter() const;

private:

	void
	addLocation(const Vector3& pos);

	std::string resourcePath_;
	boost::signals2::signal<void(void)> onUpdates_;
	Rectanguloid safetyRect_;
	Mission mission_;
	Trajectory trajectory_;
	int currentPath_;
	ControllerTarget controllerTarget_;
	SensorData sensorData_;
	VehicleOneFrame localFrame_;
};

#endif // MAPLOGIC_H
