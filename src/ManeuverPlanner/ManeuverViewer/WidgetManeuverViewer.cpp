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
		pm->subscribeOnManeuverSet([this]() -> void
								   { emit maneuverUpdated(); });
		pm->subscribeOnManeuverStatus([this]() -> void
									  { emit maneuverUpdated(); });
	}
}


void
WidgetManeuverViewer::on_update_clicked()
{
	if (auto pm = get<PlanningManager>())
	{
		pm->requestCurrentManeuver();
	}
	else
	{
		CPSLOG_ERROR << "Missing Planning Manager, cannot update list";
	}
}

void
WidgetManeuverViewer::drawManeuverSet()
{
	scene->clear();
//	const ManeuverDescriptor& currentManeuverSet = pm->getCurrentManeuverSet();
	if (auto pm = get<PlanningManager>())
	{
		const ManeuverDescriptor& currentManeuverSet = pm->getCurrentManeuverSet();
		auto activeIdx = pm->getCurrentManeuverIdx();
		if (activeIdx < 0 || currentManeuverSet.overrides.empty())
		{
			scene->addItem(new QGraphicsTextItem(tr("No Maneuver Set Active")));
			ui->currentManeuverLabel->setText(tr("N/A"));
		}
		else
		{
			if ((unsigned) activeIdx > currentManeuverSet.overrides.size())
			{
				CPSLOG_ERROR << "Current active maneuver idx is greater than number of maneuver items. Consider refreshing.";
			}

			ui->currentManeuverLabel->setText(tr(currentManeuverSet.maneuverName.data()));
			qreal lastX = 0;
			// Assuming same number of overrides and transitions, so only checking overrideIt != end
			for (auto[overrideIt, maintainIt, waveformIt, transitionIt, idx] =
				 std::tuple(currentManeuverSet.overrides.begin(), currentManeuverSet.maintains.begin(),
							currentManeuverSet.waveforms.begin(), currentManeuverSet.transitions.begin(), unsigned{0});
				 overrideIt != currentManeuverSet.overrides.end();
				 overrideIt++, maintainIt++, waveformIt++, transitionIt++, idx++)
			{
				QString text;
				if (!overrideIt->empty())
				{
					text += tr("-----------------\n");
					text += tr("Override Settings\n");
					text += tr("-----------------\n");
					for (const auto& override: *overrideIt)
					{
						text += QString::fromStdString(override.first) + tr(":") + QString::number(override.second) +
								tr("\n");
					}
				}

				if (!maintainIt->empty())
				{
					text += tr("-----------------\n");
					text += tr("Maintain Settings\n");
					text += tr("-----------------\n");
					for (const auto& maintainElem : *maintainIt)
					{
						text += (tr(maintainElem.data()) + tr("\n"));
					}
				}

				if(!waveformIt->empty())
				{
					text += tr("-----------------\n");
					text += tr("Waveform Settings\n");
					text += tr("-----------------\n");
					for (const auto& waveElem : *waveformIt)
					{
						text += (tr(waveElem.first.data()) + tr(":") + tr(waveElem.second.data()) + tr("\n"));
					}
				}

				if(!transitionIt->empty())
				{
					text += tr("-------------------\n");
					text += tr("Transition Settings\n");
					text += tr("-------------------\n");
					text += tr((transitionIt->data()));
				}

				auto rect = new QGraphicsRectItem();
				auto textItem = new QGraphicsTextItem(text, rect);
				textItem->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
				textItem->setDefaultTextColor(QColor(255, 255, 255));
				auto text_rect = textItem->boundingRect();
				rect->setRect(text_rect);
				rect->setX(lastX);
				if ((unsigned) activeIdx == idx)
				{
					//active maneuver
					rect->setBrush(QColor(0, 255, 0));
				}
				else if ((unsigned) activeIdx > idx)
				{
					//completed maneuver
					rect->setBrush(QColor(0, 0, 128));
				}
				else
				{
					//pending maneuver
					rect->setBrush(QColor(0, 128, 0));
				}
				lastX += text_rect.x() + text_rect.width() + 100;

				scene->addItem(rect);
			}
		}
	}
}
