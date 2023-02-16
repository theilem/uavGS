#ifndef WIDGETVISUALSCRIPTER_H
#define WIDGETVISUALSCRIPTER_H

#include <QWidget>
#include <cpsCore/Aggregation/AggregatableObject.hpp>

namespace Ui {
class WidgetVisualScripter;
}

class IOverridesProvider;
class WidgetMotionPrimitive;

class WidgetVisualScripter : public QWidget, public AggregatableObject<IOverridesProvider>
{
	Q_OBJECT

public:
	static constexpr auto widgetName = "visual_scripter";

	explicit WidgetVisualScripter(QWidget *parent = nullptr);
	~WidgetVisualScripter();

	inline void
	connect() {}

private slots:
	void
	on_add_clicked();

	void
	on_save_clicked();

	void
	deletePrimitive(WidgetMotionPrimitive* toDelete);

private:
	Ui::WidgetVisualScripter *ui;

	std::vector<WidgetMotionPrimitive*> primitives_;
};

#endif // WIDGETVISUALSCRIPTER_H
