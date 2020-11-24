#include <uavAP/Core/SensorData.h>
#include <uavAP/Core/Frames/InertialFrame.h>
#include <uavGS/SensorData/WidgetSensorData.h>
#include "ui_WidgetSensorData.h"

WidgetSensorData::WidgetSensorData(QWidget* parent) :
		QWidget(parent), ui(new Ui::WidgetSensorData)
{
	ui->setupUi(this);
}

WidgetSensorData::~WidgetSensorData()
{
	delete ui;
}


void
WidgetSensorData::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto sdm = get<SensorDataManager>())
	{
		sdm->subscribeOnSensorDataGlobal([this](const auto& sd)
										 { sensorDataGlobal_ = sd; emit contentUpdated(); });
		sdm->subscribeOnSensorDataLocal([this](const auto& sd)
										{ sensorDataLocal_ = sd; emit contentUpdated(); });
		sdm->subscribeOnPowerData([this](const auto& pd)
								  { powerData_ = pd; emit contentUpdated(); });
		sdm->subscribeOnServoData([this](const auto& sd)
								  { servoData_ = sd; emit contentUpdated(); });
		sdm->subscribeOnMiscValues([this](const auto& mv)
								   { miscValues_ = mv; emit contentUpdated(); });
	}
}


void
WidgetSensorData::contentUpdatedSlot()
{
	const SensorData& sd = ui->earthFrameCheckBox->isChecked() ? sensorDataGlobal_ : sensorDataLocal_;

	ui->timeValue->setText("N/A");

	ui->peValue->setText(QString::asprintf("%10.5f", sd.position.x()));
	ui->pnValue->setText(QString::asprintf("%10.5f", sd.position.y()));
	ui->puValue->setText(QString::asprintf("%10.5f", sd.position.z()));
	ui->veValue->setText(QString::asprintf("%10.5f", sd.velocity.x()));
	ui->vnValue->setText(QString::asprintf("%10.5f", sd.velocity.y()));
	ui->vuValue->setText(QString::asprintf("%10.5f", sd.velocity.z()));
	ui->vaValue->setText(QString::asprintf("%10.5f", sd.airSpeed));
	ui->vgValue->setText(QString::asprintf("%10.5f", sd.groundSpeed));
	ui->auValue->setText(QString::asprintf("%10.5f", sd.acceleration.x()));
	ui->avValue->setText(QString::asprintf("%10.5f", sd.acceleration.y()));
	ui->awValue->setText(QString::asprintf("%10.5f", sd.acceleration.z()));
	ui->rollValue->setText(QString::asprintf("%10.5f", sd.attitude.x() * 180 / M_PI));
	ui->pitchValue->setText(QString::asprintf("%10.5f", sd.attitude.y() * 180 / M_PI));
	ui->yawValue->setText(QString::asprintf("%10.5f", sd.attitude.z() * 180 / M_PI));
	ui->rollrValue->setText(QString::asprintf("%10.5f", sd.angularRate.x() * 180 / M_PI));
	ui->pitchrValue->setText(QString::asprintf("%10.5f", sd.angularRate.y() * 180 / M_PI));
	ui->yawrValue->setText(QString::asprintf("%10.5f", sd.angularRate.z() * 180 / M_PI));
	ui->voltValue->setText(QString::asprintf("%10.5f", powerData_.batteryVoltage));
	ui->currValue->setText(QString::asprintf("%10.5f", powerData_.batteryCurrent));
	ui->aileronValue->setText(QString::asprintf("%10.5f", servoData_.aileron));
	ui->elevatorValue->setText(QString::asprintf("%10.5f", servoData_.elevator));
	ui->rudderValue->setText(QString::asprintf("%10.5f", servoData_.rudder));
	ui->throttleValue->setText(QString::asprintf("%10.5f", servoData_.throttle * 100));
	ui->rpmValue->setText(QString::asprintf("%10.5f", servoData_.rpm));

	updateMiscValues();

	update();
}


void
WidgetSensorData::updateMiscValues()
{
	auto compKey = [](const auto& rhs, const auto& lhs)
	{ return rhs.first == lhs.first; };
	if (!std::equal(miscValues_.begin(), miscValues_.end(), miscValueWidget_.begin(), compKey))
	{
		for (const auto&[name, widget] : miscValueWidget_)
		{
			ui->miscValuesBox->layout()->removeWidget(widget);
			delete widget;
		}
		miscValueWidget_.clear();
		for (const auto&[name, value] : miscValues_)
		{
			auto labeledValue = new LabeledValue(name, ui->miscValuesBox);
			ui->miscValuesBox->layout()->addWidget(labeledValue);
			miscValueWidget_.emplace(std::make_pair(name, labeledValue));
		}
	}

	for (const auto& [name, value]: miscValues_)
	{
		miscValueWidget_.at(name)->set(value);
	}


}