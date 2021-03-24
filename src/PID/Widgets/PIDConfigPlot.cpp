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

	// Set title and PID params
	title = QString::fromStdString(name);
	angle_ = params.isAngle.value;
	// Maybe we want to automate this using the techniques from WidgetGeneric
	kp_ = new NamedLineEdit(params.kp.id, this);
	ki_ = new NamedLineEdit(params.ki.id, this);
	kd_ = new NamedLineEdit(params.kd.id, this);
	imax_ = new NamedLineEdit(params.imax.id, this);
	ff_ = new NamedLineEdit(params.ff.id, this);

	kp_->set(params.kp.value);
	ki_->set(params.ki.value);
	kd_->set(params.kd.value);
	imax_->set(params.imax.value);
	ff_->set(params.ff.value);

	QString stylesheet = tr("border: 1px solid gray; height:25px;");

	kp_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	ki_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	kd_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	ff_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	imax_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	kp_->setStyleSheet(stylesheet);
	ki_->setStyleSheet(stylesheet);
	kd_->setStyleSheet(stylesheet);
	ff_->setStyleSheet(stylesheet);
	imax_->setStyleSheet(stylesheet);

	ui->verticalLayout->insertWidget(1, ff_);
	ui->verticalLayout->insertWidget(1, imax_);
	ui->verticalLayout->insertWidget(1, kd_);
	ui->verticalLayout->insertWidget(1, ki_);
	ui->verticalLayout->insertWidget(1, kp_);
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
	return kp_->getDouble();
}

double
PIDConfigPlot::getkI()
{
	return ki_->getDouble();
}

double
PIDConfigPlot::getkD()
{
	return kd_->getDouble();;
}

double
PIDConfigPlot::getFF()
{
	return ff_->getDouble();
}

double
PIDConfigPlot::getIMax()
{
	return imax_->getDouble();
}

void
PIDConfigPlot::resetGraph()
{
	ui->customPlot->resetGraph();
}

void
PIDConfigPlot::setkP(double kP)
{
	kp_->set(kP);
}

void
PIDConfigPlot::setkI(double kI)
{
	ki_->set(kI);
}

void
PIDConfigPlot::setkD(double kD)
{
	kd_->set(kD);
}

void
PIDConfigPlot::setFF(double ff)
{
	ff_->set(ff);
}

void
PIDConfigPlot::setIMax(double iMax)
{
	imax_->set(iMax);
}

void
PIDConfigPlot::on_send_clicked()
{
	Control::PIDParameters a;
	a.kp = getkP();
	a.ki = getkI();
	a.kd = getkD();
	a.ff = getFF();
	a.imax = getIMax();
	a.isAngle = angle_;
	PIDTuning tune;
	tune.pid = key_;
	tune.params = a;
	if (auto pc = get<PIDConfigurator>())
	{
		pc->tunePID(tune);
	}
}

