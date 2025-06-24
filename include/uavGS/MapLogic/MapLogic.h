#ifndef MAPLOGIC_H
#define MAPLOGIC_H

#include <uavAP/MissionControl/GlobalPlanner/Trajectory.h>
#include <uavAP/MissionControl/MissionPlanner/Mission.h>
#include <uavAP/FlightControl/LocalPlanner/LinearLocalPlanner/LinearLocalPlanner.h>
#include <uavAP/Core/Frames/VehicleOneFrame.h>
#include <uavAP/Core/Rectanguloid.h>

#include "uavGS/MapLogic/MapLocation.h"
#include <cpsCore/cps_object>
#include <uavAP/Core/SensorData.h>
#include <boost/signals2/signal.hpp>

#include "uavAP/Core/DataHandling/Content.hpp"

class IScheduler;
template <typename C, typename T>
class DataHandling;
class IScheduler;
class LayoutGenerator;
class GSWidgetFactory;


struct MapLogicParams
{
	Parameter<unsigned> flightPathSize = {600, "flight_path_size", false};
	Parameter<MapLocationParams> mapCenter = {{}, "map_center", true};
	Parameter<std::string> mapTileDirectory = {"/opt/map_tiles/", "map_tile_directory", true};
	Parameter<int> maxTileDownloads = {0, "max_tile_downloads", false};

	template<typename Config>
	void
	configure(Config& c)
	{
		c & flightPathSize;
		c & mapCenter;
		c & mapTileDirectory;
		c & maxTileDownloads;
	}

};


class MapLogic: public AggregatableObject<DataHandling<Content, Target>, IScheduler, GSWidgetFactory, LayoutGenerator>, public IRunnableObject,
		public ConfigurableObject<MapLogicParams>
{
public:

	static constexpr auto typeId = "map_logic";

	MapLogic() = default;

	bool
	run(RunStage stage) override;

	const std::vector<Waypoint>&
	getWaypoints() const;

	const Trajectory&
	getPath() const;

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

	void
	downloadMapTile(int zoom, int x, int y);



	struct NamedTileDownloads
	{
		static constexpr auto name = "tile_downloads";
		NamedValue<int> remainingDownloads = {0, "remaining_downloads"};
		template <typename Configurator>
		void
		configure(Configurator& c)
		{
			c & remainingDownloads;
		}
	};

	template <typename NamedData>
	NamedData
	getNamedData() const
	{
		if constexpr (std::is_same_v<NamedData, NamedTileDownloads>)
		{
			return remainingTileDownloads_;
		}
		else
		{
			static_assert(std::is_same_v<NamedData, NamedTileDownloads>,
						  "MapLogic only provides NamedTileDownloads as named data.");
		}
		return NamedData{};
	}

	template <typename NamedData>
	void
	setNamedData(const NamedData& data)
	{
		if constexpr (std::is_same_v<NamedData, NamedTileDownloads>)
		{
			remainingTileDownloads_ = data;
		}
		else
		{
			static_assert(std::is_same_v<NamedData, NamedTileDownloads>,
						  "MapLogic only provides NamedTileDownloads as named data.");
		}
	}

private:

	void
	addLocation(const Vector3& pos);

	std::string resourcePath_;
	NamedTileDownloads remainingTileDownloads_;
	boost::signals2::signal<void(void)> onUpdates_;
	Rectanguloid safetyRect_;
	Mission mission_;
	Trajectory trajectory_;
	ControllerTarget controllerTarget_;
	SensorData sensorData_;
	VehicleOneFrame localFrame_;
};

#endif // MAPLOGIC_H
