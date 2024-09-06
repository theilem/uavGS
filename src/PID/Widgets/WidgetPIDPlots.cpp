
#include <uavGS/PID/Widgets/WidgetPIDPlots.h>
#include <uavGS/PID/Widgets/PIDCustomPlot.h>
#include "uavGS/PID/PIDConfigurator.h"
#include "ui_WidgetPIDPlots.h"

WidgetPIDPlots::WidgetPIDPlots(QWidget* parent) :
		QWidget(parent), ui(new Ui::WidgetPIDPlots)
{
	ui->setupUi(this);
	ui->columnsOnly->setChecked(true);
	this->update();
}

WidgetPIDPlots::~WidgetPIDPlots()
{
	delete ui;
}

void
WidgetPIDPlots::on_rowsOnly_pressed()
{
	clearGrid();
	int count = 0;
	for (auto& it : plots)
	{
		ui->PIDGrid->addWidget(it.second.get(), count, 0);
		it.second->setVisible(true);
		++count;
	}
	this->update();
}

void
WidgetPIDPlots::on_columnsOnly_pressed()
{
	clearGrid();
	int count = 0;
	for (auto& it : plots)
	{
		ui->PIDGrid->addWidget(it.second.get(), 0, count);
		it.second->setVisible(true);
		++count;
	}
	this->update();
}

void
WidgetPIDPlots::on_custom_pressed()
{
	clearGrid();
	int row = 0, col = 0;
	for (auto& it : plots)
	{
		ui->PIDGrid->addWidget(it.second.get(), row, col);
		it.second->setVisible(true);
		++col;
		if (col == ui->numCols->text().toInt())
		{
			col = 0;
			++row;
		}
	}
	this->update();
}

void
WidgetPIDPlots::on_numCols_valueChanged(int)
{
	ui->custom->pressed();
	ui->custom->setChecked(true);
}

void
WidgetPIDPlots::onPIDStati(const TimedPIDStati& stati)
{
	pidStati_ = stati;
	emit contentUpdated();
}

void
WidgetPIDPlots::clearGrid()
{
	for (auto& it : plots)
	{
		ui->PIDGrid->removeWidget(it.second.get());
		it.second.get()->setVisible(false);
	}
}

void
WidgetPIDPlots::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto pc = get<PIDConfigurator>())
	{
		int count = 0;
		for (auto& it : pc->getPIDMap())
		{
			//auto cp = std::make_shared<QCustomPlot>(this,it.second.name);
			auto cp = std::make_shared<PIDCustomPlot>(this, EnumMap<PIDs>::convert(it.first));
			auto* cppt = cp.get();
			ui->PIDGrid->addWidget(cppt, 0, count);
			plots.insert(std::make_pair(static_cast<int>(it.first), cp));
			++count;
		}
	}
	else
	{
		CPSLOG_ERROR << "PIDConfigurator missing.";
	}

	if (auto dh = get<DataHandling>())
	{
		dh->subscribeOnData<TimedPIDStati>(Content::PID_STATUS, [this](const auto& status){onPIDStati(status);});
	}
}


void
WidgetPIDPlots::contentUpdatedSlot()
{
	if (plots.empty())
		return;
	const auto& time = pidStati_.first;
	const auto& stati = pidStati_.second;
	for (const auto& it : stati)
	{
		auto id = static_cast<int>(it.first);
		auto plot = plots.find(id);
		if (plot == plots.end())
		{
			CPSLOG_WARN << "PID status id " << id << " does not match any plot";
			continue;
		}
		plot->second->addData(time, it.second.value, it.second.target);
	}
	this->update();
}

