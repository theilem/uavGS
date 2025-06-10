#include "uavGS/MapLogic/MapLogic.h"
#include "uavGS/GSWidgetFactory.h"

#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/MapLogic/Widgets/WidgetOverheadMap.h>
#include <uavGS/LayoutGenerator/LayoutGenerator.h>


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
            break;
        }
    case RunStage::NORMAL:
        {
            auto scheduler = get<IScheduler>();

            scheduler->schedule([this]
            {
                this->askForAll();
            }, Seconds(1));

            auto dh = get<DataHandling<Content, Target>>();
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
    if (auto dh = get<DataHandling<Content, Target>>())
    {
        dh->sendData(DataRequest::MISSION, Content::REQUEST_DATA, Target::MISSION_CONTROL);
    }
}

void
MapLogic::askForTrajectory()
{
    if (auto dh = get<DataHandling<Content, Target>>())
    {
        dh->sendData(DataRequest::TRAJECTORY, Content::REQUEST_DATA, Target::FLIGHT_CONTROL);
    }
}

void
MapLogic::askForSafetyNet()
{
    if (auto dh = get<DataHandling<Content, Target>>())
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
    if (auto dh = get<DataHandling<Content, Target>>())
    {
        dh->sendData(DataRequest::LOCAL_FRAME, Content::REQUEST_DATA, Target::MISSION_CONTROL);
    }
}

std::string
MapLogic::getMapTileDirectory() const
{
    return resourcePath_ + "/map_tiles/";
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
