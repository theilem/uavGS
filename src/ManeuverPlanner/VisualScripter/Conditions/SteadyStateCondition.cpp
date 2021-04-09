#include <uavAP/Core/SensorData.h>
#include "uavGS/ManeuverPlanner/VisualScripter/Conditions/SteadyStateCondition.h"
#include <uavGS/ManeuverPlanner/VisualScripter/SelectionValue.h>
#include <QMessageBox>
#include "ui_SteadyStateCondition.h"

SteadyStateCondition::SteadyStateCondition(QWidget* parent) :
		QWidget(parent),
		ui(new Ui::SteadyStateCondition)
{
	ui->setupUi(this);
	std::set<std::string> orderedKeys;
	for (const auto &it: EnumMap<SensorEnum>::getInstance())
	{
		orderedKeys.insert(it.second);
	}
	for (const auto &it: orderedKeys)
	{
		ui->addOption->addItem(QString::fromStdString(it));
	}
}

SteadyStateCondition::~SteadyStateCondition()
{
	delete ui;
}

void
SteadyStateCondition::on_addButton_clicked()
{
	const auto& text = ui->addOption->currentText().toStdString();
	if (values_[text])
	{
		auto d = new QMessageBox(this);
		d->setText(QString::asprintf("\"%s\" has already been added.", text.c_str()));
		d->setWindowTitle("Duplicate Entry");
		d->open();
		return;
	}
	auto s = new SelectionValue(text, this);
	QObject::connect(s, &SelectionValue::buttonClicked, this, &SteadyStateCondition::onDeleteClicked);
	auto it = values_.insert_or_assign(text, s);
	int insertionIdx = std::distance(values_.begin(), it.first);
	ui->groupBoxLayout->insertWidget(insertionIdx + 1, s);
}

void
SteadyStateCondition::onDeleteClicked(const std::string& key, SelectionValue* wid)
{
	values_[key] = nullptr;
	ui->groupBoxLayout->removeWidget(wid);
	delete wid;
}
