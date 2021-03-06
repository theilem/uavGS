﻿#ifndef WIDGETCPGRID_H
#define WIDGETCPGRID_H

#include <QWidget>
#include <memory>
#include <uavGS/PID/PIDConfigurator.h>
#include <uavGS/GSWidgetFactory.h>
#include <uavAP/Core/DataHandling/DataHandling.h>

namespace Ui
{
class WidgetCPGrid;
}

class PIDConfigPlot;

class WidgetCPGrid : public QWidget, public AggregatableObject<PIDConfigurator, GSWidgetFactory, DataHandling>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "cp_grid";

	explicit
	WidgetCPGrid(QWidget* parent = 0);

	~WidgetCPGrid();

	void
	connect();

public slots:

	void
	on_sendAllParams_clicked();

	void
	on_saveGains_clicked();

	void
	on_loadGains_clicked();

	void
	on_requestParams_clicked();

	void
	contentUpdatedSlot();

signals:

	void
	contentUpdated();

private:

	void
	onPIDStati(const PIDStati& data);

	void
	drawPlots();

	PIDStati pidStati_;
	QString title;
	Ui::WidgetCPGrid* ui;
	std::map<int, PIDConfigPlot*> plots;
};

#endif // WIDGETCPGRID_H
