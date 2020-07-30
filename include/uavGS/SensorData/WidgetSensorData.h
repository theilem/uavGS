#ifndef WIDGETSENSORDATA_H
#define WIDGETSENSORDATA_H

#include <uavAP/Core/Frames/VehicleOneFrame.h>
#include <memory>
#include <QWidget>
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include "uavGS/SensorData/SensorDataManager.h"

namespace Ui
{
class WidgetSensorData;
}

class WidgetSensorData: public QWidget, public AggregatableObject<SensorDataManager>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "sensor_data";

	explicit
	WidgetSensorData(QWidget* parent = nullptr);

	~WidgetSensorData();

	void
	connect();

private slots:

	void
	contentUpdatedSlot();

signals:

	void
	contentUpdated();

private:

	Ui::WidgetSensorData* ui;

	SensorData sensorDataGlobal_;
	SensorData sensorDataLocal_;
	PowerData powerData_;
	ServoData servoData_;
};

#endif // WIDGETSENSORDATA_H
