#include <uavGS/ManeuverPlanner/ManeuverEditor/WidgetSingletonManeuver.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/Selection.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/BooleanElement.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/FloatingElement.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/EnumElement.h>
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

	const auto& mappings = parent->getMapping();
	const auto& condMap = mappings.find("Condition");
	if (condMap != mappings.end())
	{
		conditionMappings_ = condMap->second;
	}
	else
	{
		CPSLOG_ERROR << "Cannot find condition defs!";
	}
	const auto& sigGenMap = mappings.find("SignalGenerator");
	if (sigGenMap != mappings.end())
	{
		waveformMappings_ = sigGenMap->second;
	}
	else
	{
		CPSLOG_ERROR << "Cannot find SignalGenerator defs!";
	}

	// NOTE this is sloppy and terrible OO design, but it works and is lightweight
	for (const auto& c : conditionMappings_)
	{
		ui->conditionOptions->addItem(QString::fromStdString(c.first));
	}
//	for (const auto& w : waveformMappings_)
//	{
//		ui->waveformOptions->addItem(QString::fromStdString(w.first));
//	}

	QObject::connect(ui->conditionOptions, &QComboBox::currentTextChanged, this,
					 &WidgetSingletonManeuver::conditionSelected);
	ui->conditionOptions->currentTextChanged(ui->conditionOptions->currentText());
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
		QObject::connect(sv, &SelectionValue::buttonClicked, this, &WidgetSingletonManeuver::onOverrideDeleteClicked);
		overrides_[currentText] = sv;
		insert(overrideOrder_, ui->overrideLayout, currentText, sv);
	}
	else
	{
		CPSLOG_ERROR << "No parent WidgetManeuverEditor set!";
	}
}

void
WidgetSingletonManeuver::onOverrideDeleteClicked(const std::string& key, SelectionValue* wid)
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
		QObject::connect(s, &Selection::buttonClicked, this, &WidgetSingletonManeuver::onManeuverDeleteClicked);
		maintains_[currentText] = s;
		insert(maintainOrder_, ui->maintainLayout, currentText, s);
	}
	else
	{
		CPSLOG_ERROR << "No parent WidgetManeuverEditor set!";
	}
}

void
WidgetSingletonManeuver::onManeuverDeleteClicked(const std::string& key, Selection* wid)
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
		ui->waveformOptions->addItem(QString::fromStdString(o));
	}
	auto maintains = maneuverEditor_->getMaintains();
	for (const auto& m : maintains)
	{
		ui->maintainOptions->addItem(QString::fromStdString(m));
	}
}

void
WidgetSingletonManeuver::conditionSelected(const QString& op)
{
	// Clear layout
	while (ui->conditionLayout->count() > 1)
	{
		QWidget* toDelete = ui->conditionLayout->itemAt(1)->widget();
		ui->conditionLayout->removeWidget(toDelete);
		delete toDelete;
	}

	// getConfiguration definition of selected condition
	const auto& it = conditionMappings_.find(op.toStdString());
	if (it != conditionMappings_.end())
	{
		const auto& config = maneuverEditor_->getConfiguration();
		if (const auto& configEntry = config.find(it->second); configEntry != config.end())

			for (const auto& item : configEntry->second)
			{
				const auto& typeString = item.second.get_value<std::string>();
				if (typeString == "bool")
				{
					ui->conditionLayout->addWidget(new BooleanElement(item.first, this, 1));
				}
				else if (typeString == "double" || typeString == "int")
				{
					ui->conditionLayout->addWidget(new FloatingElement(item.first, this, 1));
				}
				else if (const auto&[enumMap, mapEntry] = std::tuple{maneuverEditor_->getEnumDefs(),
																	 maneuverEditor_->getEnumDefs().find(typeString)};
						mapEntry != enumMap.end())
				{
					ui->conditionLayout->addWidget(new EnumElement(item.first, mapEntry->second, this, 1));
				}
			}
	}
	else
	{
		CPSLOG_ERROR << "No reflection information found for option" << op.toStdString();
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
