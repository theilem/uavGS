//
// Created by seedship on 11/24/20.
//

#include "uavGS/SensorData/QFI-Derived/WidgetPFD_GS.h"


WidgetPFD_GS::WidgetPFD_GS(QWidget* parent) : QWidget(parent)
{
	wPFD_ = new WidgetPFD(this);
	layout_ = new QGridLayout(this);

	layout_->setContentsMargins(0, 0, 0, 0);
	layout_->addWidget(wPFD_);

	setLayout(layout_);
}


WidgetPFD_GS::~WidgetPFD_GS()
{
	if (wPFD_)
	{
		delete wPFD_;
	}

	if (layout_)
	{
		delete layout_;
	}
}

void
WidgetPFD_GS::contentUpdatedSlot()
{
	//NOTE TODO FIXME - WidgetPFD expects imperial units... we could change the images to have SI units, or convert to imperial
	wPFD_->setAltitude(sensorData_.position.z());
	wPFD_->setAirspeed(sensorData_.airSpeed);
	wPFD_->setClimbRate(sensorData_.velocity.z());
	wPFD_->setRoll(sensorData_.attitude.x() * 180. / M_PI);
	wPFD_->setPitch(sensorData_.attitude.y() * 180. / M_PI);
	wPFD_->setHeading(sensorData_.attitude.z() * 180. / M_PI);
	wPFD_->setFlightPathMarker(sensorData_.angleOfAttack * 180. / M_PI,
			sensorData_.angleOfSideslip * 180. / M_PI);
//	wPFD_->setSlipSkid(sensorData_.angleOfSideslip * 180. / M_PI);
	wPFD_->setPressure(sensorData_.pressure);
	// http://www.aerospaceweb.org/question/atmosphere/q0126.shtml
	wPFD_->setMachNo(sensorData_.airSpeed / sqrtf(1.4 * 287 * (sensorData_.temperature + 273.15)));
	wPFD_->update();
}

void
WidgetPFD_GS::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto sdm = get<SensorDataManager>())
	{
		sdm->subscribeOnSensorDataLocal([this](const auto& sd)
										{ sensorData_ = sd; emit contentUpdated(); });
	}
}
