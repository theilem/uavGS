#include "uavGS/ManeuverPlanner/VisualScripter/SelectionValue.h"
#include "ui_SelectionValue.h"

SelectionValue::SelectionValue(const std::string& key, QWidget* parent) :
		QWidget(parent),
		ui(new Ui::SelectionValue),
		key_(key)
{
	ui->setupUi(this);
	ui->label->setText(QString::fromStdString(key));
}

SelectionValue::~SelectionValue()
{
	delete ui;
}

void
SelectionValue::on_deleteButton_clicked()
{
	emit buttonClicked(key_, this);
}

std::optional<std::pair<const std::string&, FloatingType>>
SelectionValue::get() const
{
	if (ui->value->text().isEmpty())
		return std::nullopt;
	return std::pair<const std::string&, FloatingType>(key_, ui->value->text().toDouble());
}
