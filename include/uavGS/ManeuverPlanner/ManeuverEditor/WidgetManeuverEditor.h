#ifndef WIDGETMANEUVEREDITOR_H
#define WIDGETMANEUVEREDITOR_H

#include <QWidget>
#include <cpsCore/Aggregation/AggregatableObject.hpp>

namespace Ui {
class WidgetManeuverEditor;
}

class PlanningManager;
class WidgetSingletonManeuver;

class WidgetManeuverEditor : public QWidget, public AggregatableObject<PlanningManager>
{
	Q_OBJECT
public:
	static constexpr auto widgetName = "maneuver_editor";

	explicit WidgetManeuverEditor(QWidget *parent = nullptr);
	~WidgetManeuverEditor();

	void
	connect();

	void
	deleteSingleton(WidgetSingletonManeuver * toDelete);

	const std::vector<std::string>&
	getOverrides();

	const std::vector<std::string>&
	getMaintains();

private slots:
	void
	on_add_clicked();

private:
	Ui::WidgetManeuverEditor *ui;
	std::vector<WidgetSingletonManeuver *> maneuvers_;
};

#endif // WIDGETMANEUVEREDITOR_H
