
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
	ui->tolerance->setEnabled(ui->useTolerance->isChecked());
}

SensorDataCondition::~SensorDataCondition()
{
	delete ui;
}

void
SensorDataCondition::on_useTolerance_clicked(bool checked)
{
	ui->tolerance->setEnabled(checked);
}

QJsonObject
SensorDataCondition::get() const
{
	QJsonObject ans;
	QJsonObject config;

	config["sensor"] = ui->sensorEnumOptions->currentText();
	config["relational"] = ui->relationOptions->currentText();
	config["threshold"] = ui->threshold->text().toDouble();

	config["use_tolerance"] = ui->useTolerance->isChecked();
	if (ui->useTolerance->isChecked())
	{
		config["tolerance"] = ui->tolerance->text().toDouble();
	}

	ans["sensor_data"] = config;
	return ans;
}
