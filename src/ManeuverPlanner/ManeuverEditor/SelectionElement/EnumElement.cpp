#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/EnumElement.h"
#include "ui_EnumElement.h"

EnumElement::EnumElement(const std::string& name, const std::unordered_set<std::string>& enum_keys, QWidget* parent, int level) :
	QWidget(parent),
	ui(new Ui::EnumElement)
{
	ui->setupUi(this);
	ui->label->setText(QString::fromStdString(makeTitle(name, level)));

	for(const auto& key : enum_keys)
	{
		ui->value->addItem(QString::fromStdString(key));
	}

	QObject::connect(ui->value, &QComboBox::currentTextChanged, this,
					 &EnumElement::on_conditionSelected);
}

EnumElement::~EnumElement()
{
	delete ui;
}

std::pair<std::string, std::string>
EnumElement::get()
{
	return std::pair<std::string, std::string>(ui->label->text().toStdString(), ui->value->currentText().toStdString());
}

void
EnumElement::on_conditionSelected(const QString& op)
{
	emit conditionSelected(op.toStdString());
}
