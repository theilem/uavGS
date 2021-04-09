#include <uavGS/ManeuverPlanner/VisualScripter/SelectionValue.h>
#include <uavGS/ManeuverPlanner/VisualScripter/Selection.h>
#include "uavGS/ManeuverPlanner/VisualScripter/WidgetMotionPrimitive.h"
#include "ui_WidgetMotionPrimitive.h"

#include <QMessageBox>
#include <uavGS/ManeuverPlanner/VisualScripter/Conditions/DurationCondition.h>
#include <uavGS/ManeuverPlanner/VisualScripter/Conditions/SensorDataCondition.h>
#include <uavGS/ManeuverPlanner/VisualScripter/Conditions/SteadyStateCondition.h>
#include <uavGS/ManeuverPlanner/VisualScripter/Waveforms/WaveformWrapper.h>

WidgetMotionPrimitive::WidgetMotionPrimitive(const PlanningManager& pm, int index, QWidget* parent) :
		QWidget(parent),
		ui(new Ui::WidgetMotionPrimitive),
		pm_(pm)
{
	ui->setupUi(this);
	setIdx(index);


	auto overrides = pm.getOverrides();
	for (const auto& o : overrides)
	{
		ui->overrideOptions->addItem(QString::fromStdString(o));
		ui->waveformOptions->addItem(QString::fromStdString(o));
	}
	auto maintains = pm.getMaintains();
	for (const auto& m : maintains)
	{
		ui->maintainsOptions->addItem(QString::fromStdString(m));
	}
}

WidgetMotionPrimitive::~WidgetMotionPrimitive()
{
	delete ui;
}

void
WidgetMotionPrimitive::setIdx(int index)
{
	ui->mainGroupBox->setTitle(QString::asprintf("Maneuver Element %u", index + 1));
}

bool
WidgetMotionPrimitive::_checkDuplicate(const std::string& key)
{
	if (presentOverrides_.find(key) != presentOverrides_.end()){
		auto d = new QMessageBox(this);
		d->setText(QString::asprintf("Each field can either be added to overrides or waveforms, but not both. \"%s\" has already been added.", key.c_str()));
		d->setWindowTitle("Duplicate Entry");
		d->open();
		return true;
	}
	return false;
}

template <typename T>
void
WidgetMotionPrimitive::_insertOverride(std::map<std::string, T>& map, QVBoxLayout* layout, const std::string& text, T wid)
{
	const auto &insertItr = map.insert_or_assign(text, wid);
	int insertionIdx = std::distance(map.begin(), insertItr.first);
	// Idx 0 contains the widget with the selector
	layout->insertWidget(1 + insertionIdx, wid);
	presentOverrides_.insert(text);
}

template <typename T>
void
WidgetMotionPrimitive::_delete(std::map<std::string, T>& map, QLayout* layout, const std::string& text, QWidget* wid)
{
	presentOverrides_.erase(text);
	map[text] = nullptr;
	layout->removeWidget(wid);
	delete wid;
}

void
WidgetMotionPrimitive::on_close_clicked()
{
	emit closeClicked(this);
}

void
WidgetMotionPrimitive::on_addOverride_clicked()
{
	auto currentText = ui->overrideOptions->currentText().toStdString();
	if (_checkDuplicate(currentText))
		return;
	auto sv = new SelectionValue(currentText, this);
	QObject::connect(sv, &SelectionValue::buttonClicked, this, &WidgetMotionPrimitive::onOverrideDeleteClicked);
	_insertOverride(overrideOrder_, ui->overrideLayout, currentText, sv);
}

void
WidgetMotionPrimitive::onOverrideDeleteClicked(const std::string& key, SelectionValue* wid)
{
	_delete(overrideOrder_, ui->overrideLayout, key, wid);
}

void
WidgetMotionPrimitive::on_addWaveform_clicked()
{
	auto currentText = ui->waveformOptions->currentText().toStdString();
	if (_checkDuplicate(currentText))
		return;
	auto ww = new WaveformWrapper(currentText, this);
	QObject::connect(ww, &WaveformWrapper::buttonClicked, this, &WidgetMotionPrimitive::onWaveformDeleteClicked);
	_insertOverride(waveformOrder_, ui->waveformLayout, currentText, ww);
}

void
WidgetMotionPrimitive::onWaveformDeleteClicked(const std::string& key, WaveformWrapper* wid)
{
	_delete(waveformOrder_, ui->waveformLayout, key, wid);
}

void
WidgetMotionPrimitive::on_addMaintains_clicked()
{
	const auto& currentText = ui->maintainsOptions->currentText().toStdString();
	if (maintainOrder_[currentText]) {
		auto d = new QMessageBox(this);
		d->setText("This maintain has already been added.");
		d->setWindowTitle("Duplicate Entry");
		d->open();
		return;
	}
	auto s = new Selection(currentText, this);
	QObject::connect(s, &Selection::buttonClicked, this, &WidgetMotionPrimitive::onMaintainDeleteClicked);
	const auto &insertItr = maintainOrder_.insert_or_assign(currentText, s);
	int insertionIdx = std::distance(maintainOrder_.begin(), insertItr.first);
	// Idx 0 contains the widget with the selector
	ui->maintainsLayout->insertWidget(1 + insertionIdx, s);
}


void
WidgetMotionPrimitive::onMaintainDeleteClicked(const std::string& key, Selection* wid)
{
	_delete(maintainOrder_, ui->maintainsLayout, key, wid);
}

void
WidgetMotionPrimitive::on_selectCondition_clicked()
{
	//item 0 is the selector
	while(ui->conditionLayout->count() > 1) {
		auto wid = ui->conditionLayout->itemAt(1)->widget();
		ui->conditionLayout->removeWidget(wid);
		delete wid;
	}
	const auto& conditionOp = ui->conditionOptions->currentText().toStdString();
	if (conditionOp == "Duration Condition") {
		auto dc = new DurationCondition(this);
		ui->conditionLayout->insertWidget(1, dc);
	} else if (conditionOp == "Sensor Data Condition") {
		auto sdc = new SensorDataCondition(this);
		ui->conditionLayout->insertWidget(1, sdc);
	} else if (conditionOp == "Steady State Condition") {
		auto ssc = new SteadyStateCondition(this);
		ui->conditionLayout->insertWidget(1, ssc);
	} else {
		CPSLOG_ERROR << "Unknown condition type!";
	}
}
