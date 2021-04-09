#include "uavGS/ManeuverPlanner/VisualScripter/Conditions/SensorDataCondition.h"
#include "ui_SensorDataCondition.h"

#include <cpsCore/Utilities/EnumMap.hpp>
#include <uavAP/Core/SensorData.h>
#include <uavAP/FlightAnalysis/Condition/Relational.h>
#include <set>

SensorDataCondition::SensorDataCondition(QWidget* parent) :
		QWidget(parent),
		ui(new Ui::SensorDataCondition)
{
	ui->setupUi(this);

	std::set<std::string> orderedKeys;
	for (const auto &it: EnumMap<SensorEnum>::getInstance())
	{
		orderedKeys.insert(it.second);
	}
	for (const auto &it: orderedKeys)
	{
		ui->sensorEnumOptions->addItem(QString::fromStdString(it));
	}
	for (const auto &it: EnumMap<Relational>::getInstance())
	{
		ui->relationOptions->addItem(QString::fromStdString(it.second));
	}
}

SensorDataCondition::~SensorDataCondition()
{
	delete ui;
}
