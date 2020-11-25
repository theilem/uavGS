//
// Created by seedship on 11/25/20.
//

#ifndef UAVGS_WIDGETSIX_GS_H
#define UAVGS_WIDGETSIX_GS_H

#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/SensorData.h>

#include "QFlightInstruments/WidgetSix.h"
#include "uavGS/SensorData/SensorDataManager.h"

namespace Ui
{
class WidgetSix_GS;
}

class WidgetSix_GS : public QWidget, public AggregatableObject<SensorDataManager>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "six";

	explicit
	WidgetSix_GS(QWidget* parent = 0);

	virtual
	~WidgetSix_GS();

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
	WidgetSix* wSix_;
	QGridLayout* layout_;
};


#endif //UAVGS_WIDGETSIX_GS_H
