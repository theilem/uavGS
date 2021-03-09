//
// Created by seedship on 2/25/21.
//

#ifndef UAVGS_WIDGETMANEUVEREDITOR_H
#define UAVGS_WIDGETMANEUVEREDITOR_H


#include <QGraphicsScene>


class QToolBox;

class WidgetManeuverViewer: public QGraphicsScene
{
	Q_OBJECT
public:
	WidgetManeuverViewer() = default;


private:
	Ui::WidgetManeuverViewer* ui;
	QToolBox* toolBox_;
};


#endif //UAVGS_WIDGETMANEUVEREDITOR_H
