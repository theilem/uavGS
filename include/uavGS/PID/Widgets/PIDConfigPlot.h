#ifndef PIDCONFIGPLOT_H
#define PIDCONFIGPLOT_H

#include <QWidget>
#include "uavAP/FlightControl/Controller/PIDController/PIDHandling.h"
#include <cpsCore/Aggregation/AggregatableObject.hpp>

#include "uavGS/ParameterSets/NamedLineEdit.h"
#include "uavGS/ParameterSets/NamedCheckbox.h"
#include "uavGS/PID/PIDConfigurator.h"

namespace Ui
{
class PIDConfigPlot;
}

class PIDConfigPlot: public QWidget, public AggregatableObject<PIDConfigurator>
{
Q_OBJECT

	friend class WidgetCPGrid;

public:

	PIDConfigPlot(QWidget* parent);

	void
	setParams(PIDs pid, const Control::PIDParameters& params);

	void
	setData(double current, double target);

	void
	sendData();

	void
	connect();

	double
	getkP();

	double
	getkI();

	double
	getkD();

	double
	getFF();

	double
	getIMax();

	void
	resetGraph();

	void
	setkP(double kP);

	void
	setkI(double kI);

	void
	setkD(double kD);

	void
	setFF(double ff);

	void
	setIMax(double iMax);

	~PIDConfigPlot();

private slots:

	void
	on_send_clicked();

private:

	NamedLineEdit* kp_;
	NamedLineEdit* ki_;
	NamedLineEdit* kd_;
	NamedLineEdit* imax_;
	NamedLineEdit* ff_;
	bool angle_;

	Ui::PIDConfigPlot* ui;
	int key_;
	QString title;
};

#endif // PIDCONFIGPLOT_H
