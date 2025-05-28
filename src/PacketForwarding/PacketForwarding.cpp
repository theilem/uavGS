//
// Created by mirco on 06.07.20.
//

#include <cpsCore/Utilities/Packet.h>
#include <cpsCore/Utilities/IDC/IDC.h>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include "uavGS/ParameterSets/WidgetConfigurableObject.h"
#include "uavGS/PacketForwarding/PacketForwarding.h"
#include "uavGS/GSWidgetFactory.h"

bool
PacketForwarding::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSetAll())
			{
				CPSLOG_ERROR << "Missing deps";
				return true;
			}

			auto wf = get<GSWidgetFactory>();
			wf->registerWidget<WidgetConfigurableObject<PacketForwarding>>("packet_forwarding");
			auto idc = get<IDC>();
			packetForwarding_ = idc->createSender("forwarding");
			forwardedToAutopilot_ = idc->createSender("autopilot");

		}
		case RunStage::NORMAL:
		{
			auto dh = get<EnumBasedDataHandling>();

			dh->subscribeOnPackets([this](const auto& p){forwardPacket(p);});

			auto idc = get<IDC>();
			idc->subscribeOnPacket("forwarding", [this](const auto& packet){onForwardingPacket(packet);});

		}
		default:
			break;
	}
	return false;
}

void
PacketForwarding::forwardPacket(const Packet& packet)
{
	if (params.sendData())
	{
		packetForwarding_.sendPacket(packet);
	}
}

void
PacketForwarding::onForwardingPacket(const Packet& packet)
{
	if (params.receiveData())
	{
		forwardedToAutopilot_.sendPacket(packet);
	}
}
