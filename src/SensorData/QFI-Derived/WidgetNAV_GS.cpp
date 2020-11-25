//
// Created by seedship on 11/25/20.
//

#include "uavGS/SensorData/QFI-Derived/WidgetNAV_GS.h"

WidgetNAV_GS::WidgetNAV_GS(QWidget* parent) : QWidget(parent)
{
	wNav_ = new WidgetNAV(this);
	layout_ = new QGridLayout(this);

	layout_->setContentsMargins(0, 0, 0, 0);
	layout_->addWidget(wNav_);

	setLayout(layout_);
}


WidgetNAV_GS::~WidgetNAV_GS()
{
	if (wNav_)
	{
		delete wNav_;
	}

	if (layout_)
	{
		delete layout_;
	}
}

void
WidgetNAV_GS::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto sdm = get<SensorDataManager>())
	{
		sdm->subscribeOnSensorDataLocal([this](const auto& sd)
										{ sensorData_ = sd; emit contentUpdated(); });
	}
}

void
WidgetNAV_GS::contentUpdatedSlot()
{
	wNav_->setHeading(sensorData_.attitude.z() * 180. / M_PI);
}
