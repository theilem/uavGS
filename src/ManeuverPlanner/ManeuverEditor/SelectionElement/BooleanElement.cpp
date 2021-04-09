#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/BooleanElement.h"
#include "ui_BooleanElement.h"

BooleanElement::BooleanElement(const std::string& name, QWidget *parent, int level) :
	QWidget(parent),
	ui(new Ui::BooleanElement)
{
	ui->setupUi(this);
	ui->label->setText(QString::fromStdString(makeTitle(name, level)));
}

BooleanElement::~BooleanElement()
{
	delete ui;
}

std::pair<std::string, bool>
BooleanElement::get()
{
	return std::pair<std::string, bool>(ui->label->text().toStdString(), ui->value->isChecked());
}
