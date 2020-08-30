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
	QString t;
	const SensorData& sd = ui->earthFrameCheckBox->isChecked() ? sensorDataGlobal_ : sensorDataLocal_;


	t = QString::fromStdString("N/A");
	ui->timeValue->setText(t);

	t.sprintf("%10.5f", sd.position.x());
	ui->peValue->setText(t);

	t.sprintf("%10.5f", sd.position.y());
	ui->pnValue->setText(t);

	t.sprintf("%10.5f", sd.position.z());
	ui->puValue->setText(t);

	t.sprintf("%10.5f", sd.velocity.x());
	ui->veValue->setText(t);

	t.sprintf("%10.5f", sd.velocity.y());
	ui->vnValue->setText(t);

	t.sprintf("%10.5f", sd.velocity.z());
	ui->vuValue->setText(t);

	t.sprintf("%10.5f", sd.airSpeed);
	ui->vaValue->setText(t);

	t.sprintf("%10.5f", sd.groundSpeed);
	ui->vgValue->setText(t);

	t.sprintf("%10.5f", sd.acceleration.x());
	ui->auValue->setText(t);

	t.sprintf("%10.5f", sd.acceleration.y());
	ui->avValue->setText(t);

	t.sprintf("%10.5f", sd.acceleration.z());
	ui->awValue->setText(t);

	t.sprintf("%10.5f", sd.attitude.x() * 180 / M_PI);
	ui->rollValue->setText(t);

	t.sprintf("%10.5f", sd.attitude.y() * 180 / M_PI);
	ui->pitchValue->setText(t);

	t.sprintf("%10.5f", sd.attitude.z() * 180 / M_PI);
	ui->yawValue->setText(t);

	t.sprintf("%10.5f", sd.angularRate.x() * 180 / M_PI);
	ui->rollrValue->setText(t);

	t.sprintf("%10.5f", sd.angularRate.y() * 180 / M_PI);
	ui->pitchrValue->setText(t);

	t.sprintf("%10.5f", sd.angularRate.z() * 180 / M_PI);
	ui->yawrValue->setText(t);

	t.sprintf("%10.5f", powerData_.batteryVoltage);
	ui->voltValue->setText(t);

	t.sprintf("%10.5f", powerData_.batteryCurrent);
	ui->currValue->setText(t);

	t.sprintf("%10.5f", servoData_.aileron);
	ui->aileronValue->setText(t);

	t.sprintf("%10.5f", servoData_.elevator);
	ui->elevatorValue->setText(t);

	t.sprintf("%10.5f", servoData_.rudder);
	ui->rudderValue->setText(t);

	t.sprintf("%10.5f", servoData_.throttle * 100);
	ui->throttleValue->setText(t);

	t.sprintf("%10.5f", servoData_.rpm);
	ui->rpmValue->setText(t);


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