#include "uavGS/ManeuverPlanner/VisualScripter/Selection.h"
#include "ui_Selection.h"

Selection::Selection(const std::string& key, QWidget* parent) :
		QWidget(parent),
		ui(new Ui::Selection),
		key_(key)
{
	ui->setupUi(this);
	ui->label->setText(QString::fromStdString(key));
}

Selection::~Selection()
{
	delete ui;
}

const std::string&
Selection::get() const
{
	return key_;
}

void
Selection::on_deleteButton_clicked()
{
	emit buttonClicked(key_, this);
}
