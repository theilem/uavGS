#include "uavGS/LayoutGenerator/Widgets/WidgetLoader.h"
#include "ui_WidgetLoader.h"

WidgetLoader::WidgetLoader(QWidget *parent) :
		QWidget(parent), ui(new Ui::WidgetLoader), layout(NULL), tab(NULL), index(-1)
{
	ui->setupUi(this);
}

void
WidgetLoader::linkLayoutGenerator(const ChangeWidget& changeWidget,
		const ChangeLayout &changeLayout, const std::vector<std::string> &widgets)
{
	changeWidget_ = changeWidget;
	changeLayout_ = changeLayout;
	for (auto name : widgets)
	{
		ui->widgetSelector->addItem(QString::fromStdString(name));
	}
}

void
WidgetLoader::setLayoutHandle(QGridLayout * inLayout)
{
	layout = inLayout;
}

QGridLayout *
WidgetLoader::getLayoutHandle()
{
	return layout;
}

void
WidgetLoader::setTabHandle(QTabWidget * inTab)
{
	tab = inTab;
}

QTabWidget *
WidgetLoader::getTabHandle()
{
	return tab;
}

void
WidgetLoader::setTabIndex(int indexIn)
{
	index = indexIn;
}

int
WidgetLoader::getTabIndex()
{
	return index;
}

WidgetLoader::~WidgetLoader()
{
	delete ui;
}

void
WidgetLoader::on_widgetButton_clicked()
{
	changeWidget_(this, ui->widgetSelector->currentText());
}

void
WidgetLoader::on_layoutButton_clicked()
{
	if (ui->gridLayout->isChecked())
		changeLayout_(this, "grid_layout", ui->numRows->value(), ui->numCols->value());
	else
		changeLayout_(this, "tab_layout", ui->numTabs->value(), ui->numCols->value()); //numCols unused

}
