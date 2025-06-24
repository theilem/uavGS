#include "uavGS/MapLogic/MapLogic.h"
#include "uavGS/GSWidgetFactory.h"

#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/MapLogic/Widgets/WidgetOverheadMap.h>
#include <uavGS/LayoutGenerator/LayoutGenerator.h>
#include "uavGS/Widgets/WidgetNamedData.hpp"

#include <curl/curl.h>


bool
MapLogic::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSet<DataHandling<Content, Target>, IScheduler, GSWidgetFactory>())
			{
				CPSLOG_ERROR << "MapLogic: missing dependencies.";
				return true;
			}
			if (auto lg = get<LayoutGenerator>())
			{
				resourcePath_ = lg->getResourcePath();
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<WidgetOverheadMap>();
			wf->registerWidget<WidgetNamedData<NamedTileDownloads, MapLogic>>("tile_downloads");
			remainingTileDownloads_.remainingDownloads = params.maxTileDownloads();
			break;
		}
		case RunStage::NORMAL:
		{
			auto scheduler = get<IScheduler>();

			scheduler->schedule([this]
			{
				this->askForAll();
			}, Seconds(1));

			auto dh = get<DataHandling<Content, Target> >();
			dh->subscribeOnData<Mission>(Content::MISSION, [this](const auto& m)
			{
				this->mission_ = m;
				onUpdates_();
			});
			dh->subscribeOnData<Trajectory>(Content::TRAJECTORY, [this](const auto& t)
			{
				this->trajectory_ = t;
				onUpdates_();
			});
			dh->subscribeOnData<Rectanguloid>(Content::SAFETY_BOUNDS, [this](const auto& sb)
			{
				this->safetyRect_ = sb;
				onUpdates_();
			});
			break;
		}
		default:
			break;
	}
	return false;
}

const std::vector<Waypoint>&
MapLogic::getWaypoints() const
{
	return mission_.waypoints();
}

const Trajectory&
MapLogic::getPath() const
{
	return trajectory_;
}

void
MapLogic::askForAll()
{
	askForMission();
	askForTrajectory();
	askForSafetyNet();
	askForLocalFrame();
}

void
MapLogic::askForMission()
{
	if (auto dh = get<DataHandling<Content, Target> >())
	{
		dh->sendData(DataRequest::MISSION, Content::REQUEST_DATA, Target::MISSION_CONTROL);
	}
}

void
MapLogic::askForTrajectory()
{
	if (auto dh = get<DataHandling<Content, Target> >())
	{
		dh->sendData(DataRequest::TRAJECTORY, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
	}
}

void
MapLogic::askForSafetyNet()
{
	if (auto dh = get<DataHandling<Content, Target> >())
	{
		dh->sendData(DataRequest::SAFETY_BOUNDS, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
	}
}

void
MapLogic::setSafetyBounds(const Rectanguloid& rect)
{
	safetyRect_ = rect;
}

const Rectanguloid&
MapLogic::getSafetyBounds() const
{
	return safetyRect_;
}

const SensorData&
MapLogic::getSensorData() const
{
	return sensorData_;
}

void
MapLogic::askForLocalFrame()
{
	if (auto dh = get<DataHandling<Content, Target> >())
	{
		dh->sendData(DataRequest::LOCAL_FRAME, Content::REQUEST_DATA, Target::MISSION_CONTROL);
	}
}

std::string
MapLogic::getMapTileDirectory() const
{
	return params.mapTileDirectory();
}

std::string
MapLogic::getIconPath() const
{
	return resourcePath_ + "/icons/";
}

void
MapLogic::setLocalFrame(const VehicleOneFrame& frame)
{
	localFrame_ = frame;
}

boost::signals2::connection
MapLogic::subscribeOnUpdates(const boost::signals2::signal<void(void)>::slot_type& slot)
{
	return onUpdates_.connect(slot);
}

MapLocation
MapLogic::getMapCenter() const
{
	return MapLocation(params.mapCenter());
}

void
MapLogic::downloadMapTile(int zoom, int x, int y)
{
	if (remainingTileDownloads_.remainingDownloads() <= 0)
	{
		CPSLOG_DEBUG << "MapLogic: No more tile downloads allowed.";
		return;
	}
	remainingTileDownloads_.remainingDownloads()--;
	CURL* curl = curl_easy_init();
	if (!curl)
	{
		CPSLOG_ERROR << "MapLogic: Failed to initialize CURL.";
		return;
	}
	std::string outputFile = params.mapTileDirectory() + "/" + std::to_string(zoom) + "/" + std::to_string(x) + "/" +
	                         std::to_string(y) + ".jpg";
	// create the directory first
	std::filesystem::create_directories(std::filesystem::path(outputFile).parent_path());
	FILE* file = fopen(outputFile.c_str(), "wb");
	if (!file)
	{
		CPSLOG_ERROR << "MapLogic: Failed to open file for writing: " << outputFile;
		curl_easy_cleanup(curl);
		return;
	}

	std::string url = "https://services.arcgisonline.com/ArcGIS/rest/services/"
	                  "World_Imagery/MapServer/tile/" +
	                  std::to_string(zoom) + "/" +
	                  std::to_string(y) + "/" +
	                  std::to_string(x);

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	fclose(file);
	if (res != CURLE_OK)
	{
		CPSLOG_ERROR << "MapLogic: Failed to download tile from " << url << ": " << curl_easy_strerror(res);
	}
}
