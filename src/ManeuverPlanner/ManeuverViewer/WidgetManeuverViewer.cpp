//
// Created by seedship on 2/25/21.
//

#include <QtWidgets>
#include "ui_WidgetManeuverViewer.h"
#include "uavGS/ManeuverPlanner/ManeuverViewer/WidgetManeuverViewer.h"
#include "uavGS/ManeuverPlanner/PlanningManager.h"

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
		if (activeIdx < 0 || currentManeuverSet.second.empty())
		{
			scene->addItem(new QGraphicsTextItem(tr("No Maneuver Set Active")));
			ui->currentManeuverLabel->setText(tr("N/A"));
		}
		else
		{
			if ((unsigned) activeIdx > currentManeuverSet.second.size())
			{
				CPSLOG_ERROR
						<< "Current active maneuver idx is greater than number of maneuver items. Consider refreshing.";
			}

			ui->currentManeuverLabel->setText(tr(currentManeuverSet.first.data()));
			qreal lastX = 0;
			// Assuming same number of overrides and transitions, so only checking overrideIt != end
			for (auto[it, idx] = std::tuple(currentManeuverSet.second.begin(), unsigned{0});
				 it != currentManeuverSet.second.end(); it++, idx++)
			{
				QString text;
				if (!it->overrides.value.empty())
				{
					text += tr("-----------------\n");
					text += tr("Override Settings\n");
					text += tr("-----------------\n");
					for (const auto& override: it->overrides.value)
					{
						text += QString::fromStdString(override.first) + tr(":") + QString::number(override.second) +
								tr("\n");
					}
				}

				if (!it->maintains.value.empty())
				{
					text += tr("-----------------\n");
					text += tr("Maintain Settings\n");
					text += tr("-----------------\n");
					for (const auto& maintainElem : it->maintains.value)
					{
						text += (tr(maintainElem.data()) + tr("\n"));
					}
				}

				if (!it->waveforms.value.empty())
				{
					text += tr("-----------------\n");
					text += tr("Waveform Settings\n");
					text += tr("-----------------\n");
					for (const auto& waveElem : it->waveforms.value)
					{
						std::stringstream ss;
						boost::property_tree::write_json(ss, waveElem.second);
						text += (tr(waveElem.first.data()) + tr(":") + tr(ss.str().c_str()));
					}
				}

				if (!it->transition.value.empty())
				{
					text += tr("-------------------\n");
					text += tr("Transition Settings\n");
					text += tr("-------------------\n");
					std::stringstream ss;
					boost::property_tree::write_json(ss, it->transition.value);
					text += tr(ss.str().c_str());
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
