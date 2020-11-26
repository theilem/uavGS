//
// Created by seedship on 11/25/20.
//

#include "uavGS/SensorData/QFI-Derived/WidgetSix_GS.h"

WidgetSix_GS::WidgetSix_GS(QWidget* parent) : QWidget(parent)
{
	wSix_ = new WidgetSix(this);
	layout_ = new QGridLayout(this);

	layout_->setContentsMargins(0, 0, 0, 0);
	layout_->addWidget(wSix_);

	setLayout(layout_);
}

WidgetSix_GS::~WidgetSix_GS()
{
	if (wSix_)
	{
		delete wSix_;
	}

	if (layout_)
	{
		delete layout_;
	}
}

void
WidgetSix_GS::contentUpdatedSlot()
{
	//NOTE TODO FIXME - WidgetSix expects imperial units... we could change the images to have SI units, or convert to imperial
	wSix_->setAltitude(sensorData_.position.z());
	wSix_->setAirspeed(sensorData_.airSpeed);
	wSix_->setClimbRate(sensorData_.velocity.z());
	wSix_->setRoll(sensorData_.attitude.x() * 180. / M_PI);
	wSix_->setPitch(sensorData_.attitude.y() * 180. / M_PI);
	wSix_->setHeading(sensorData_.attitude.z() * 180. / M_PI);
	wSix_->setTurnRate(sensorData_.angularRate.z() * 180. / M_PI);
	wSix_->setSlipSkid(sensorData_.angleOfSideslip * 180. / M_PI); /** slip/skid ball angle [deg] */
	wSix_->setPressure(sensorData_.pressure);
	wSix_->update();
}

void
WidgetSix_GS::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto sdm = get<SensorDataManager>())
	{
		sdm->subscribeOnSensorDataLocal([this](const auto& sd)
										{ sensorData_ = sd; emit contentUpdated(); });
	}
}
