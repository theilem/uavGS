#include "uavGS/ManeuverPlanner/VisualScripter/Conditions/DurationCondition.h"
#include "ui_DurationCondition.h"

DurationCondition::DurationCondition(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DurationCondition)
{
	ui->setupUi(this);
}

DurationCondition::~DurationCondition()
{
	delete ui;
}
