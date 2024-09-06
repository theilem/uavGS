#ifndef WIDGETPIDPLOTS_H
#define WIDGETPIDPLOTS_H

#include <QWidget>
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavAP/FlightControl/Controller/PIDController/PIDHandling.h>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include "uavGS/PID/PIDConfigurator.h"

class PIDCustomPlot;
namespace Ui
{
class WidgetPIDPlots;
}

class WidgetPIDPlots : public QWidget, public AggregatableObject<PIDConfigurator, DataHandling>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "pid_plots";

	explicit
	WidgetPIDPlots(QWidget* parent = nullptr);

	~WidgetPIDPlots();

	void
	connect();

private slots:

	void
	on_rowsOnly_pressed();

	void
	on_columnsOnly_pressed();

	void
	on_custom_pressed();

	void
	on_numCols_valueChanged(int arg1);

	void
	contentUpdatedSlot();

signals:

	void
	contentUpdated();

private:

	void
	onPIDStati(const TimedPIDStati& data);

	void
	clearGrid();

	TimedPIDStati pidStati_;
	Ui::WidgetPIDPlots* ui;
	std::map<int, std::shared_ptr<PIDCustomPlot>> plots;
};

#endif // WIDGETPIDPLOTS_H
