//
// Created by seedship on 2/25/21.
//

#include <QtWidgets>
#include "ui_WidgetManeuverViewer.h"
#include "uavGS/ManeuverPlanner/ManeuverViewer/WidgetManeuverViewer.h"

WidgetManeuverViewer::WidgetManeuverViewer(QWidget* parent) :
		QWidget(parent), ui(new Ui::WidgetManeuverViewer)
{
	ui->setupUi(this);
}

WidgetManeuverViewer::~WidgetManeuverViewer()
{
	delete ui;
}

void
WidgetManeuverViewer::connect()
{
//	if (auto pm = get<PlanningManager>())
//	{
//
//	}
//	if (auto wf = get<GSWidgetFactory>())
//	{
//
//	}
}


void WidgetManeuverViewer::on_update_clicked()
{
	if (auto pm = get<PlanningManager>()) {

	} else {
		CPSLOG_ERROR << "Missing Planning Manager, cannot update list";
	}
}