//
// Created by seedship on 2/25/21.
//

#ifndef UAVGS_WIDGETMANEUVERVIEWER_H
#define UAVGS_WIDGETMANEUVERVIEWER_H


#include <QGraphicsScene>
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/GSWidgetFactory.h>

namespace Ui {
class WidgetManeuverViewer;
}

class QGraphicsScene;

class WidgetManeuverViewer: public QWidget, public AggregatableObject<PlanningManager, GSWidgetFactory>
{
	Q_OBJECT
public:

	static constexpr auto widgetName = "maneuver_viewer";

	WidgetManeuverViewer(QWidget* parent = nullptr);

	~WidgetManeuverViewer();

	void
	connect();

private slots:
	void
	on_update_clicked();

	void
	drawManeuverSet();

signals:
	void maneuverUpdated();

private:
	QGraphicsScene *scene;
	Ui::WidgetManeuverViewer* ui;
};


#endif //UAVGS_WIDGETMANEUVERVIEWER_H
