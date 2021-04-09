#ifndef WIDGETMANEUVEREDITOR_H
#define WIDGETMANEUVEREDITOR_H

#include <QWidget>
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include "uavGS/ManeuverPlanner/ManeuverEditor/Reflection/ReflectionInterpreter.h"

namespace Ui
{
class WidgetManeuverEditor;
}

class PlanningManager;

class WidgetSingletonManeuver;

class WidgetManeuverEditor: public QWidget, public AggregatableObject<PlanningManager, ReflectionInterpreter>
{
Q_OBJECT
public:
	static constexpr auto widgetName = "maneuver_editor";

	explicit WidgetManeuverEditor(QWidget* parent = nullptr);

	~WidgetManeuverEditor();

	void
	connect();

	void
	deleteSingleton(WidgetSingletonManeuver* toDelete);

	const std::vector<std::string>&
	getOverrides() const;

	const std::vector<std::string>&
	getMaintains() const;

	const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
	getMapping() const;

	const std::unordered_map<std::string, Configuration>&
	getConfiguration() const;

	const std::unordered_map<std::string, std::unordered_set<std::string>>&
	getEnumDefs() const;

private slots:

	void
	on_add_clicked();

private:
	Ui::WidgetManeuverEditor* ui;
	std::vector<WidgetSingletonManeuver*> maneuvers_;
};

#endif // WIDGETMANEUVEREDITOR_H
