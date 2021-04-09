#include <uavGS/ManeuverPlanner/VisualScripter/WidgetMotionPrimitive.h>
#include "uavGS/ManeuverPlanner/VisualScripter/WidgetVisualScripter.h"
#include "ui_WidgetVisualScripter.h"

WidgetVisualScripter::WidgetVisualScripter(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetVisualScripter)
{
	ui->setupUi(this);
}

WidgetVisualScripter::~WidgetVisualScripter()
{
	delete ui;
}

void
WidgetVisualScripter::on_add_clicked()
{
	if (auto pm = get<PlanningManager>()){
		auto wmp = new WidgetMotionPrimitive(*pm, primitives_.size(), this);
		primitives_.push_back(wmp);
		QObject::connect(wmp, &WidgetMotionPrimitive::closeClicked, this, &WidgetVisualScripter::deletePrimitive);
		ui->scrollLayout->addWidget(wmp);
	} else {
		CPSLOG_ERROR << "Missing Planning Manager!";
	}
}

void
WidgetVisualScripter::deletePrimitive(WidgetMotionPrimitive* toDelete)
{
	ui->scrollLayout->removeWidget(toDelete);
	auto deleteIdx = std::find(primitives_.begin(), primitives_.end(), toDelete);
	if (deleteIdx != primitives_.end())
	{
		primitives_.erase(deleteIdx);
		delete toDelete;

	}
	else
		CPSLOG_ERROR << "deletePrimitive called on unknown entry";

	unsigned renumIdx = deleteIdx - primitives_.begin();
	for (auto idx = renumIdx; idx < primitives_.size(); idx++)
	{
		primitives_[idx]->setIdx(idx);
	}
}
