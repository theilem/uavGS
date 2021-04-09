#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/FloatingElement.h"
#include "ui_FloatingElement.h"

FloatingElement::FloatingElement(const std::string& name, QWidget *parent, int level) :
	QWidget(parent),
	ui(new Ui::FloatingElement)
{
	ui->setupUi(this);
	ui->label->setText(QString::fromStdString(makeTitle(name, level)));
}

FloatingElement::~FloatingElement()
{
	delete ui;
}

std::pair<std::string, FloatingType>
FloatingElement::get()
{
	return std::pair<std::string, FloatingType>(ui->label->text().toStdString(), ui->value->text().toDouble());
}
