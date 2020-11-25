//
// Created by seedship on 11/24/20.
//

#ifndef UAVGS_WIDGETPFD_GS_H
#define UAVGS_WIDGETPFD_GS_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/SensorData.h>

#include "QFlightInstruments/WidgetPFD.h"
#include "uavGS/SensorData/SensorDataManager.h"

namespace Ui
{
class WidgetPFD_GS;
}

class WidgetPFD_GS : public QWidget, public AggregatableObject<SensorDataManager>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "pfd";

	explicit
	WidgetPFD_GS(QWidget* parent = 0);

	virtual
	~WidgetPFD_GS();

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
	WidgetPFD* wPFD_;
	QGridLayout* layout_;
};


#endif //UAVGS_WIDGETPFD_GS_H
