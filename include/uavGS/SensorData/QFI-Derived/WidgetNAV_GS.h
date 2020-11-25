//
// Created by seedship on 11/25/20.
//

#ifndef UAVGS_WIDGETNAV_GS_H
#define UAVGS_WIDGETNAV_GS_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/SensorData.h>

#include "QFlightInstruments/WidgetNAV.h"
#include "uavGS/SensorData/SensorDataManager.h"

namespace Ui
{
class WidgetNAV_GS;
}

class WidgetNAV_GS : public QWidget, public AggregatableObject<SensorDataManager>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "nav";

	explicit
	WidgetNAV_GS(QWidget* parent = 0);

	virtual
	~WidgetNAV_GS();

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
	WidgetNAV* wNav_;
	QGridLayout* layout_;

};


#endif //UAVGS_WIDGETNAV_GS_H
