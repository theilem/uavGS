#ifndef PIDCUSTOMPLOT_H
#define PIDCUSTOMPLOT_H

#include "uavGS/PID/Widgets/QCustomPlot.h"
#include <cpsCore/Aggregation/AggregatableObject.hpp>

class PIDConfigurator;

class PIDCustomPlot : public QCustomPlot
{
public:
	PIDCustomPlot(QWidget* = 0, std::string name = "UNDEFINED");

	void
	setTitle(std::string title);

	void
	addData(double current, double target);

	void
	resetGraph();

	void
	setPixelsPerDataPoint(double resolution);

protected:
	void
	resizeEvent(QResizeEvent* event) override;

	void
	mouseReleaseEvent(QMouseEvent*) override;

private:
	double pixelsPerDataPoint_;
	QCPTextElement plotTitle_;
	QCPGraph* currentGraph_;
	QCPGraph* targetGraph_;
	double maxValue;
	double minValue;
};

#endif // PIDCUSTOMPLOT_H
