//
// Created by Mirco Theile on 8/6/25.
//

#ifndef XPLANEMANAGER_H
#define XPLANEMANAGER_H
#include <cpsCore/cps_object>

#include "cpsCore/Utilities/IPC/Publisher.h"

class DataPresentation;
class GSWidgetFactory;
class IPC;

struct XPlaneManagerParams
{
    Parameter<Vector3> homePosition = {{0.0, 0.0, 0.0}, "home_position", true}; // Lat, long, altitude
    template <class Configurator>
    void
    configure(Configurator& c)
    {
        c & homePosition;
    }
};

struct Teleportation
{
    Parameter<Vector3> position = {{0.0, 0.0, 0.0}, "position", true}; // Lat, long, altitude
    Parameter<Vector3> velocity = {{0.0, 0.0, 0.0}, "velocity", true};
    Parameter<Vector3> attitude = {{0.0, 0.0, 0.0}, "attitude", true};
    template <class Configurator>
    void
    configure(Configurator& c)
    {
        c & position;
        c & velocity;
        c & attitude;
    }
};

class XPlaneManager : public ConfigurableObject<XPlaneManagerParams>,
                      public AggregatableObject<IPC, GSWidgetFactory, DataPresentation>,
                      public IRunnableObject
{
public:
    static constexpr auto typeId = "xplane_manager";
    XPlaneManager() = default;
    ~XPlaneManager() override = default;
    bool
    run(RunStage stage) override;
    void
    setWidgetData(const Teleportation& teleportation);
    Teleportation
    getWidgetData() const;
private:
    Publisher<Packet> xplanePublisher_;
};

#endif //XPLANEMANAGER_H
