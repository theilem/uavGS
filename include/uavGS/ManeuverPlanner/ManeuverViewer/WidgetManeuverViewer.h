//
// Created by seedship on 2/25/21.
//

#ifndef UAVGS_WIDGETMANEUVEREDITOR_H
#define UAVGS_WIDGETMANEUVEREDITOR_H


#include <QGraphicsScene>
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <uavGS/GSWidgetFactory.h>

namespace Ui {
class WidgetManeuverViewer;
}

class QToolBox;

class WidgetManeuverViewer: public QWidget, public AggregatableObject<PlanningManager, GSWidgetFactory>
{
	Q_OBJECT
public:
	WidgetManeuverViewer(QWidget* parent = nullptr);

	~WidgetManeuverViewer();

	void
	connect();

private slots:
	void on_update_clicked();

private:
	Ui::WidgetManeuverViewer* ui;
};


#endif //UAVGS_WIDGETMANEUVEREDITOR_H
