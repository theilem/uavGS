
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

QJsonObject
DurationCondition::get() const
{
	QJsonObject config;
	QJsonObject ans;
	config["duration"] = ui->duration->text().toDouble();
	ans["duration"] = config;
	return ans;
}
