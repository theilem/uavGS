//
// Created by Mirco Theile on 8/6/25.
//

#include <cpsCore/Utilities/IDC/Header/Hash.h>
#include "uavGS/XPlane/XPlaneManager.h"
#include "uavGS/ParameterSets/WidgetParameterStruct.h"

#include "uavGS/GSWidgetFactory.h"
#include "cpsCore/Utilities/IPC/IPC.h"
#include "cpsCore/Utilities/DataPresentation/DataPresentation.h"

bool
XPlaneManager::run(RunStage stage)
{
    switch (stage)
    {
    case RunStage::INIT:
        {
            if (!checkIsSetAll())
            {
                CPSLOG_ERROR << "XPlaneManager: Failed to run init!";
                return true;
            }
            auto widgetFactory = get<GSWidgetFactory>();
            widgetFactory->registerWidget<WidgetParameterStruct<Teleportation, XPlaneManager>>(
                "xplane_teleportation_widget");
            break;
        }
    case RunStage::NORMAL:
        {
            auto ipc = get<IPC>();

            auto options = IPCOptions{};
            options.multiTarget = false;
            xplanePublisher_ = ipc->publishPackets("uavgs_to_uavee", options);
            break;
        }
    case RunStage::FINAL:
    default:
        break;
    }
    return false;
}

void
XPlaneManager::setWidgetData(const Teleportation& teleportation)
{
    CPSLOG_DEBUG << "XPlaneManager: Setting widget data for teleportation.";
    std::cout << "XPlaneManager: Teleportation position: " << teleportation.position().transpose()
        << ",\n velocity: " << teleportation.velocity().transpose()
        << ",\n attitude: " << teleportation.attitude().transpose() << std::endl;

    auto dp = get<DataPresentation>();
    auto packet = dp->serialize(teleportation);
    dp->addHeader(packet, Hash("xplane_teleportation"));
    xplanePublisher_.publish(packet);
}

Teleportation
XPlaneManager::getWidgetData() const
{
    Teleportation tele;
    tele.position = params.homePosition();
    tele.velocity = {0.0, 0.0, 0.0};
    tele.attitude = {0.0, 0.0, 0.0};
    return tele;
}
