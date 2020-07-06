//
// Created by mirco on 06.07.20.
//

#ifndef UAVGS_PACKETFORWARDING_H
#define UAVGS_PACKETFORWARDING_H

#include <cpsCore/cps_object>
#include "uavGS/PacketForwarding/PacketForwardingParams.h"

class IDC;

class DataHandling;
class GSWidgetFactory;

class PacketForwarding
		: public AggregatableObject<IDC, DataHandling, GSWidgetFactory>,
		  public IRunnableObject,
		  public ConfigurableObject<PacketForwardingParams>
{
public:

	static constexpr TypeId typeId = "packet_forwarding";

	bool
	run(RunStage stage) override;

private:

	void
	forwardPacket(const Packet& packet);

	IDCSender packetForwarding_;

};

#endif //UAVGS_PACKETFORWARDING_H
