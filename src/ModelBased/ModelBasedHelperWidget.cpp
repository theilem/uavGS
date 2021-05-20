//
// Created by seedship on 5/17/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ModelBasedWidget.h" resolved

#include <uavAP/Core/DataHandling/DataHandling.h>

#include "uavGS/ModelBased/ModelBasedHelperWidget.h"
#include "ui_ModelBasedHelperWidget.h"

ModelBasedHelperWidget::ModelBasedHelperWidget(QWidget* parent) :
		QWidget(parent), ui(new Ui::ModelBasedHelperWidget)
{
	ui->setupUi(this);
}

ModelBasedHelperWidget::~ModelBasedHelperWidget()
{
	delete ui;
}

void
ModelBasedHelperWidget::on_clearLQRIntegrators_clicked()
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(true, Content::CLEAR_LQR_INTEGRATOR, Target::FLIGHT_CONTROL);
	}
	else
	{
		CPSLOG_ERROR << "Missing DataHandling, cannot message autopilot!";
	}
}

void
ModelBasedHelperWidget::on_stopSafetyController_clicked()
{
	if (auto dh = get<DataHandling>())
	{
		dh->sendData(true, Content::DISABLE_SAFETY_CONTROLLER, Target::FLIGHT_CONTROL);
	}
	else
	{
		CPSLOG_ERROR << "Missing DataHandling, cannot message autopilot!";
	}
}
