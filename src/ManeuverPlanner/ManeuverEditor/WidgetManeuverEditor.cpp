#include "uavGS/ManeuverPlanner/ManeuverEditor/WidgetManeuverEditor.h"
#include "uavGS/ManeuverPlanner/ManeuverEditor/WidgetSingletonManeuver.h"
#include "uavGS/ManeuverPlanner/PlanningManager.h"
#include "ui_WidgetManeuverEditor.h"

WidgetManeuverEditor::WidgetManeuverEditor(QWidget* parent) :
		QWidget(parent),
		ui(new Ui::WidgetManeuverEditor)
{
	ui->setupUi(this);
}

WidgetManeuverEditor::~WidgetManeuverEditor()
{
	delete ui;
}

void
WidgetManeuverEditor::connect()
{

}

void
WidgetManeuverEditor::on_add_clicked()
{
	auto wS = new WidgetSingletonManeuver(this);
	wS->setIdx(maneuvers_.size());
	maneuvers_.push_back(wS);
	ui->scrollLayout->addWidget(wS);
}

void
WidgetManeuverEditor::deleteSingleton(WidgetSingletonManeuver* toDelete)
{
	ui->scrollLayout->removeWidget(toDelete);
	auto deleteIdx = std::find(maneuvers_.begin(), maneuvers_.end(), toDelete);
	maneuvers_.erase(deleteIdx);
	delete toDelete;

	unsigned renumIdx = deleteIdx - maneuvers_.begin();
	for (auto idx = renumIdx; idx < maneuvers_.size(); idx++)
	{
		maneuvers_[idx]->setIdx(idx);
	}
}

const std::vector<std::string>&
WidgetManeuverEditor::getOverrides() const
{
	if (auto pm = get<PlanningManager>())
	{
		return pm->getOverrides();
	} else
	{
		CPSLOG_ERROR << "Missing PlanningManager";
		throw std::runtime_error("Missing PlanningManager");
	}
}

const std::vector<std::string>&
WidgetManeuverEditor::getMaintains() const
{
	if (auto pm = get<PlanningManager>())
	{
		return pm->getMaintains();
	} else
	{
		CPSLOG_ERROR << "Missing PlanningManager";
		throw std::runtime_error("Missing PlanningManager");
	}
}

const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
WidgetManeuverEditor::getMapping() const
{
	if (auto ri = get<ReflectionInterpreter>())
	{
		return ri->getMapping();
	} else
	{
		CPSLOG_ERROR << "Missing ReflectionInterpreter";
		throw std::runtime_error("Missing ReflectionInterpreter");
	}
}

const std::unordered_map<std::string, Configuration>&
WidgetManeuverEditor::getConfiguration() const
{
	if (auto ri = get<ReflectionInterpreter>())
	{
		return ri->getConfiguration();
	} else
	{
		CPSLOG_ERROR << "Missing ReflectionInterpreter";
		throw std::runtime_error("Missing ReflectionInterpreter");
	}
}

const std::unordered_map<std::string, std::unordered_set<std::string>>&
WidgetManeuverEditor::getEnumDefs() const
{
	if (auto ri = get<ReflectionInterpreter>())
	{
		return ri->getEnumDefs();
	} else
	{
		CPSLOG_ERROR << "Missing ReflectionInterpreter";
		throw std::runtime_error("Missing ReflectionInterpreter");
	}
}
