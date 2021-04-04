#ifndef WIDGETSINGLETONMANEUVER_H
#define WIDGETSINGLETONMANEUVER_H

#include <QWidget>
#include <cpsCore/Utilities/LinearAlgebra.h>
#include <unordered_set>
#include <set>
#include <QVBoxLayout>

#include "uavGS/ManeuverPlanner/ManeuverEditor/WidgetManeuverEditor.h"

namespace Ui
{
class WidgetSingletonManeuver;
}

class SelectionValue;

class Selection;

class WidgetSingletonManeuver: public QWidget
{
Q_OBJECT

public:
	explicit WidgetSingletonManeuver(WidgetManeuverEditor* parent = nullptr);

	~WidgetSingletonManeuver();

	void
	setIdx(unsigned idx);

private slots:

	// Deletes itself (this instance)
	void
	on_deleteButton_clicked();

	void
	on_addOverride_clicked();

	void
	onButtonClickedOverride(const std::string& key, SelectionValue* wid);

	void
	on_addMaintain_clicked();

	void
	onButtonClickedMaintain(const std::string& key, Selection* wid);

private:

	void
	refreshData();


	Ui::WidgetSingletonManeuver* ui;
	WidgetManeuverEditor* maneuverEditor_;

	//keeps track of which overrides are added in the widget
	std::unordered_map<std::string, QWidget*> overrides_;
	//std::set maintains a self balancing tree which allows us to insert in alphabetical order
	std::set<std::string> overrideOrder_;


	std::unordered_map<std::string, QWidget*> maintains_;
	std::set<std::string> maintainOrder_;
};

#endif // WIDGETSINGLETONMANEUVER_H
