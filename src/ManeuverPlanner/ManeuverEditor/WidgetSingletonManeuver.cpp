#include <uavGS/ManeuverPlanner/ManeuverEditor/WidgetSingletonManeuver.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/Selection.h>
#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionValue.h"
#include "ui_WidgetSingletonManeuver.h"


void
insert(std::set<std::string>& set, QVBoxLayout* layout, const std::string& text, QWidget* wid);

WidgetSingletonManeuver::WidgetSingletonManeuver(WidgetManeuverEditor* parent) :
		QWidget(parent),
		ui(new Ui::WidgetSingletonManeuver),
		maneuverEditor_(parent)
{
	ui->setupUi(this);
	refreshData();
}

WidgetSingletonManeuver::~WidgetSingletonManeuver()
{
	delete ui;
}

void
WidgetSingletonManeuver::setIdx(unsigned int idx)
{
	ui->setLabel->setText(QString::asprintf("Set #%u", idx));
}

void
WidgetSingletonManeuver::on_deleteButton_clicked()
{
	if (maneuverEditor_)
	{
		maneuverEditor_->deleteSingleton(this);
	}
	else
	{
		CPSLOG_ERROR << "No parent WidgetManeuverEditor set!";
	}
}

void
WidgetSingletonManeuver::on_addOverride_clicked()
{
	if (maneuverEditor_)
	{
		auto currentText = ui->overrideOptions->currentText().toStdString();
		if (overrides_[currentText])
			return;
		auto sv = new SelectionValue(currentText, this);
		QObject::connect(sv, &SelectionValue::buttonClicked, this, &WidgetSingletonManeuver::onButtonClickedOverride);
		overrides_[currentText] = sv;
		insert(overrideOrder_, ui->overrideLayout, currentText, sv);
	}
	else
	{
		CPSLOG_ERROR << "No parent WidgetManeuverEditor set!";
	}
}

void
WidgetSingletonManeuver::onButtonClickedOverride(const std::string& key, SelectionValue* wid)
{
	// Delete selected SelectionValue
	overrideOrder_.erase(key);
	overrides_[key] = nullptr;
	ui->overrideLayout->removeWidget(wid);
	delete wid;
}

void
WidgetSingletonManeuver::on_addMaintain_clicked()
{
	if (maneuverEditor_)
	{
		auto currentText = ui->maintainOptions->currentText().toStdString();
		if (maintains_[currentText])
			return;
		auto s = new Selection(currentText, this);
		QObject::connect(s, &Selection::buttonClicked, this, &WidgetSingletonManeuver::onButtonClickedMaintain);
		maintains_[currentText] = s;
		insert(maintainOrder_, ui->maintainLayout, currentText, s);
	}
	else
	{
		CPSLOG_ERROR << "No parent WidgetManeuverEditor set!";
	}
}

void
WidgetSingletonManeuver::onButtonClickedMaintain(const std::string& key, Selection* wid)
{
	// Delete selected Selection
	maintainOrder_.erase(key);
	maintains_[key] = nullptr;
	ui->maintainLayout->removeWidget(wid);
	delete wid;
}

// NOTE: currently does not support uavAP changing. We would need to clear existing entries that are no longer defined
// Thus, this function is currently private and only reserved as a constructor helper
void
WidgetSingletonManeuver::refreshData()
{
	auto overrides = maneuverEditor_->getOverrides();
	for (const auto& o : overrides)
	{
		ui->overrideOptions->addItem(QString::fromStdString(o));
	}
	auto maintains = maneuverEditor_->getMaintains();
	for (const auto& m : maintains)
	{
		ui->maintainOptions->addItem(QString::fromStdString(m));
	}
}

void
insert(std::set<std::string>& set, QVBoxLayout* layout, const std::string& text, QWidget* wid)
{
	auto insertItr = set.insert(text);
	int insertionIdx = std::distance(set.begin(), insertItr.first);
	// Idx 0 contains the widget with the selector
	layout->insertWidget(1 + insertionIdx, wid);
}
