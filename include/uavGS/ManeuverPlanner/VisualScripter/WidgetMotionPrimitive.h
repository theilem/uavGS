#ifndef WIDGETMOTIONPRIMITIVE_H
#define WIDGETMOTIONPRIMITIVE_H

#include <QWidget>
#include <unordered_set>
#include <map>

#include <uavGS/ManeuverPlanner/PlanningManager.h>
#include "IGettableJsonObject.h"

namespace Ui {
class WidgetMotionPrimitive;
}

class SelectionValue;
class Selection;
class WaveformWrapper;
class QVBoxLayout;

class WidgetMotionPrimitive : public QWidget
{
	Q_OBJECT

public:
	explicit WidgetMotionPrimitive(const IOverridesProvider& pm, int index, QWidget *parent = nullptr);
	~WidgetMotionPrimitive();

	void
	setIdx(int index);

	QJsonObject
	get();

signals:
	void
	closeClicked(WidgetMotionPrimitive* wid);

private:
	Ui::WidgetMotionPrimitive *ui;

	bool
	_checkDuplicate(const std::string& key);

	template <typename T>
	void
	_insertOverride(std::map<std::string, T>& map, QVBoxLayout* layout, const std::string& text, T wid);

	template <typename T>
	void
	_delete(std::map<std::string, T>& map, QLayout* layout, const std::string& text, QWidget* wid);

	//keeps track of which overrides are added in the widget
	std::unordered_set<std::string> presentOverrides_;

	// TODO: I thought std::distance on rb iterators was O(1) but it is O(n). Although runtime complexity doesnt really
	// matter because we shouldn't have that many elements.
	std::map<std::string, SelectionValue* > overrideOrder_;
	std::map<std::string, WaveformWrapper* > waveformOrder_;
	std::map<std::string, Selection*> maintainOrder_;

	IGettableJsonObject* condition_;

private slots:
	void
	on_close_clicked();

	void
	on_addOverride_clicked();

	void
	onOverrideDeleteClicked(const std::string& key, SelectionValue* wid);

	void
	on_addWaveform_clicked();

	void
	onWaveformDeleteClicked(const std::string& key, WaveformWrapper* wid);

	void
	on_addMaintains_clicked();

	void
	onMaintainDeleteClicked(const std::string& key, Selection* wid);

	void
	on_selectCondition_clicked();
};

#endif // WIDGETMOTIONPRIMITIVE_H
