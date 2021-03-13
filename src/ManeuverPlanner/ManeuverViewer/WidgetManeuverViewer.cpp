//
// Created by seedship on 2/25/21.
//

#include <QtWidgets>
#include "ui_WidgetManeuverViewer.h"
#include "uavGS/ManeuverPlanner/ManeuverViewer/WidgetManeuverViewer.h"

WidgetManeuverViewer::WidgetManeuverViewer(QWidget* parent) :
		QWidget(parent), scene(new QGraphicsScene), ui(new Ui::WidgetManeuverViewer)
{
	ui->setupUi(this);
	ui->graphicsView->setScene(scene);

}

WidgetManeuverViewer::~WidgetManeuverViewer()
{
	//https://doc.qt.io/qt-5/qgraphicsview.html#setScene -- view does not take ownership of scene so it must be deleted
	delete ui->graphicsView->scene();
	delete ui;
}

void
WidgetManeuverViewer::connect()
{
	QObject::connect(this, SIGNAL(maneuverUpdated()), this, SLOT(drawManeuverSet()));
	if (auto pm = get<PlanningManager>())
	{
		drawManeuverSet();
		pm->subscribeOnManeuverSet([this](const auto& maneuverSet)
		{
			emit maneuverUpdated();
		});
	}
//	if (auto wf = get<GSWidgetFactory>())
//	{
//
//	}
}



void WidgetManeuverViewer::on_update_clicked()
{
	if (auto pm = get<PlanningManager>()) {
		pm->requestCurrentManeuver();
	} else {
		CPSLOG_ERROR << "Missing Planning Manager, cannot update list";
	}
}

void
WidgetManeuverViewer::drawManeuverSet()
{
	scene->clear();
	if (auto pm = get<PlanningManager>())
	{
		const std::vector<ManeuverOverride>& currentManeuverSet = pm->getCurrentManeuverSet();
		if (currentManeuverSet.empty())
		{
			scene->addItem(new QGraphicsTextItem(tr("No Maneuver Set Active")));
		}
		else
		{
			qreal lastX = 0;
			for(const auto &maneuver : currentManeuverSet)
			{
				QString text;
				for(const auto &override: maneuver)
				{
					if (!text.isEmpty()) {
						text += tr("\n");
					}
					text += QString::fromStdString(override.first) + tr(":") + QString::number(override.second);
				}
				auto rect = new QGraphicsRectItem();
				auto textItem = new QGraphicsTextItem(text, rect);
				auto text_rect = textItem->boundingRect();
				rect->setRect(text_rect);
				rect->setX(lastX);

				lastX += text_rect.x() + text_rect.width() + 100;

				scene->addItem(rect);
			}
		}
	}
}
