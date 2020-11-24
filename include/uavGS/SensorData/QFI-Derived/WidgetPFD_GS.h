//
// Created by seedship on 11/24/20.
//

#ifndef UAVGS_WIDGETPFD_GS_H
#define UAVGS_WIDGETPFD_GS_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/SensorData.h>

#include "QFlightInstruments/example/WidgetPFD.h"
#include "uavGS/SensorData/SensorDataManager.h"


class WidgetPFD_GS : public AggregatableObject<SensorDataManager>, public QWidget
{
Q_OBJECT
public:
	void
	connect();

private slots:

	void
	contentUpdatedSlot();

signals:

	void
	contentUpdated();

private:
	SensorData sensorData_;
	WidgetPFD wPFD_;
};


#endif //UAVGS_WIDGETPFD_GS_H
