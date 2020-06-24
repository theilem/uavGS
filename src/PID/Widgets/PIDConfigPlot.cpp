#include <uavGS/PID/Widgets/PIDConfigPlot.h>
#include <uavGS/PID/PIDConfigurator.h>
#include "ui_PIDConfigPlot.h"

PIDConfigPlot::PIDConfigPlot(QWidget *parent) :
		QWidget(parent), ui(new Ui::PIDConfigPlot), key_(0)
{
	ui->setupUi(this);
	//ui->customPlot->setTitle(QString::fromStdString(name));
}

void
PIDConfigPlot::setParams(PIDs pid, const Control::PIDParameters& params)
{
	key_ = static_cast<int>(pid);
	auto name = EnumMap<PIDs>::convert(pid);
	ui->customPlot->setTitle(name);
	//ui->customPlot->setTname
	title = QString::fromStdString(name);
	// Set title and PID params
	ui->kP->setText(QString::number(params.kp()));
	ui->kP->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
	ui->kI->setText(QString::number(params.ki()));
	ui->kI->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
	ui->kD->setText(QString::number(params.kd()));
	ui->kD->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
	ui->IMax->setText(QString::number(params.imax()));
	ui->IMax->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
	ui->FF->setText(QString::number(params.ff()));
	ui->FF->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
}

PIDConfigPlot::~PIDConfigPlot()
{
	delete ui;
}

void
PIDConfigPlot::setData(double current, double target)
{
	ui->customPlot->addData(current, target);
}

void
PIDConfigPlot::sendData()
{
	on_send_clicked();
}

void
PIDConfigPlot::connect()
{

}

double
PIDConfigPlot::getkP()
{
	return ui->kP->text().toDouble();
}

double
PIDConfigPlot::getkI()
{
	return ui->kI->text().toDouble();
}

double
PIDConfigPlot::getkD()
{
	return ui->kD->text().toDouble();;
}

double
PIDConfigPlot::getFF()
{
	return ui->FF->text().toDouble();
}

double
PIDConfigPlot::getIMax()
{
	return ui->IMax->text().toDouble();
}

void
PIDConfigPlot::resetGraph()
{
	ui->customPlot->resetGraph();
}

void
PIDConfigPlot::setkP(double kP)
{
	ui->kP->setText(QString::number(kP));
}

void
PIDConfigPlot::setkI(double kI)
{
	ui->kI->setText(QString::number(kI));
}

void
PIDConfigPlot::setkD(double kD)
{
	ui->kD->setText(QString::number(kD));
}

void
PIDConfigPlot::setFF(double ff)
{
	ui->FF->setText(QString::number(ff));
}

void
PIDConfigPlot::setIMax(double iMax)
{
	ui->IMax->setText(QString::number(iMax));
}

void
PIDConfigPlot::on_send_clicked()
{
	Control::PIDParameters a;
	a.kp = ui->kP->text().toDouble();
	a.ki = ui->kI->text().toDouble();
	a.kd = ui->kD->text().toDouble();
	a.ff = ui->FF->text().toDouble();
	a.imax = ui->IMax->text().toDouble();
	PIDTuning tune;
	tune.pid = key_;
	tune.params = a;
	if (auto pc = get<PIDConfigurator>())
	{
		pc->tunePID(tune);
	}
}

