#include <boost/property_tree/json_parser.hpp>
#include <uavAP/Core/DataHandling/DataHandling.h>
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenuBar>
#include <QTabWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QtWidgets/QScrollArea>

#include "uavGS/LayoutGenerator/LayoutGenerator.h"
#include "uavGS/LayoutGenerator/Widgets/WidgetLoader.h"
#include "uavGS/GSWidgetFactory.h"
#include "uavGS/LayoutGenerator/Widgets/WidgetMainWindow.h"

LayoutGenerator::~LayoutGenerator()
{
	handleQuit();
}

bool
LayoutGenerator::run(RunStage stage)
{
	switch (stage)
	{
		case RunStage::INIT:
		{
			if (!checkIsSet<GSWidgetFactory>())
			{
				CPSLOG_ERROR << "LayoutGenerator: Missing dependencies";
				return true;
			}
			break;
		}
		case RunStage::NORMAL:
		{
			makeScrollableWin(createLayout(params, nullptr));
			break;
		}
		case RunStage::FINAL:
		{
			for (auto it : windows_)
			{
				if (!it)
					continue;
				it->show();
			}
			break;
		}
		default:
			break;
	}
	return false;
}

void
LayoutGenerator::changeLayout(WidgetLoader* wid, const QString& type, int rows, int cols)
{
	auto wf = get<GSWidgetFactory>();
	if (!wf)
	{
		CPSLOG_ERROR << "Cannot create widget because widget factory is missing";
		return;
	}
	QGridLayout* grid = wid->getLayoutHandle(); //TODO get rid of layout handle and look at parent instead
	if (type == "grid_layout")
	{
		QGridLayout* ngrid = new QGridLayout();
		for (int x = 0; x < rows; x++)
		{
			for (int y = 0; y < cols; y++)
			{
				WidgetLoader* wl = new WidgetLoader();
				wl->setLayoutHandle(ngrid);
				ngrid->addWidget(wl, x, y);
				wl->linkLayoutGenerator(
						std::bind(&LayoutGenerator::changeWidget, this, std::placeholders::_1,
								  std::placeholders::_2),
						std::bind(&LayoutGenerator::changeLayout, this, std::placeholders::_1,
								  std::placeholders::_2, std::placeholders::_3,
								  std::placeholders::_4), wf->getWidgetTypes());
			}
		}
		QWidget* wGrid = new QWidget();
		wGrid->setLayout(ngrid);
		grid->replaceWidget(wid, wGrid);
		wid->close();
	}
	else if (type == "tab_layout")
	{
		QTabWidget* tabs = new QTabWidget();
		for (int i = 0; i < rows; i++)
		{
			QString name = "Tab " + QString::number(i + 1);
			WidgetLoader* wl = new WidgetLoader();
			tabs->addTab(wl, name);
			wl->setTabIndex(i);
			wl->linkLayoutGenerator(
					std::bind(&LayoutGenerator::changeWidget, this, std::placeholders::_1,
							  std::placeholders::_2),
					std::bind(&LayoutGenerator::changeLayout, this, std::placeholders::_1,
							  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
					wf->getWidgetTypes());
			wl->setTabHandle(tabs);
			wl->setLayoutHandle(grid);
		}
		grid->replaceWidget(wid, tabs);
		wid->close();
	}
}

void
LayoutGenerator::changeWidget(WidgetLoader* wid, const QString& type)
{
	auto wf = get<GSWidgetFactory>();
	if (!wf)
	{
		CPSLOG_ERROR << "Cannot create widget because widget factory is missing";
		return;
	}
	QGridLayout* grid = wid->getLayoutHandle(); //TODO get rid of layout handle and look at parent instead
	if (type == "blank")
	{
		wid->close();
	}
	else
	{
		QWidget* newWidget = wf->createWidget(type.toStdString(), wid->parentWidget());
		if (wid->getTabHandle() == nullptr)
			grid->replaceWidget(wid, newWidget);
		else
		{
			wid->getTabHandle()->addTab(newWidget, type);
			wid->getTabHandle()->removeTab(0);
		}
		wid->close();
	}
	delete wid;

}

QWidget*
LayoutGenerator::createLayout(const LayoutParams& p, QWidget* parent)
{
	auto wf = get<GSWidgetFactory>();
	if (!wf)
	{
		CPSLOG_ERROR << "Cannot create widget because widget factory is missing";
		return nullptr;
	}
	if (p.widget().empty())
	{
		switch (p.layout())
		{
			case Layouts::HORIZONTAL:
			{
				QWidget* wHBox = new QWidget(parent);
				QHBoxLayout* hbox = new QHBoxLayout(wHBox);
				for (const auto& item : p.items())
				{
					//QJsonObject item = items.at(i).toObject();
					QWidget* wItem = createLayout(item, wHBox);
					hbox->addWidget(wItem);
				}
				wHBox->setLayout(hbox);
				return wHBox;
			}
			case Layouts::VERTICAL:
			{
				QWidget* wVBox = new QWidget(parent);
				QVBoxLayout* vbox = new QVBoxLayout(wVBox);
				for (const auto& item : p.items())
				{
					//QJsonObject item = items.at(i).toObject();
					QWidget* wItem = createLayout(item, wVBox);
					vbox->addWidget(wItem);
				}

				wVBox->setLayout(vbox);
				return wVBox;
			}
			case Layouts::QUAD:
			{
				QWidget* wGrid = new QWidget(parent);
				QGridLayout* grid(new QGridLayout(wGrid));

				if (p.items().size() > 4)
				{
					CPSLOG_ERROR << "Cannot handle more than 4 items in QUAD layout";
					return nullptr;
				}
				int k = 0;
				for (const auto& item : p.items())
				{
					auto l = createLayout(item, wGrid);
					grid->addWidget(l, k / 2, k % 2);
					k++;
				}
				wGrid->setLayout(grid);
				return wGrid;
			}
			case Layouts::TAB:
			{
				QTabWidget* wTabs = new QTabWidget();
				int k = 0;
				for (const auto& item : p.items())
				{
					QWidget* wTab = createLayout(item, parent);
					wTabs->addTab(wTab, QString::fromStdString("tab " + std::to_string(k + 1)));
					k++;
				}
				return wTabs;
			}
		}
	}
	return wf->createWidget(p.widget(), parent);
}

void
LayoutGenerator::makeScrollableWin(QWidget* win)
{
	auto wf = get<GSWidgetFactory>();
	if (!wf)
		return;
	auto mainWin = new WidgetMainWindow;
	wf->connectWidget(mainWin);
	QScrollArea* scrollArea(new QScrollArea(mainWin));
	scrollArea->setWidgetResizable(true);
	win->setParent(scrollArea);
	scrollArea->setWidget(win);
	mainWin->setCentralWidget(scrollArea);
	mainWin->show();
	windows_.push_back(mainWin);
}

void
LayoutGenerator::handleQuit()
{
	for (auto win : windows_)
	{
		if (!win)
			continue;
		win->close();
		delete win;
	}
}

void
LayoutGenerator::addWin()
{
	CPSLOG_TRACE << "Prompting for config file.";
	QFileDialog dialog;
	dialog.setWindowTitle("Open Configuration");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	QString confPath;
	if (dialog.exec())
	{
		confPath = dialog.selectedFiles().front();
	}
	else
	{
		CPSLOG_TRACE << "Cancelled config loading.";
		return;
	}
	Configuration config;
	boost::property_tree::read_json(confPath.toStdString(), config);
	PropertyMapper<Configuration> pm(config);
	LayoutParams p;
	p.configure(pm);
	makeScrollableWin(createLayout(p, nullptr));
}

void
LayoutGenerator::addWidget()
{
	auto wf = get<GSWidgetFactory>();
	if (!wf)
	{
		CPSLOG_ERROR << "Widget factory missing";
		return;
	}
	QGridLayout* grid(new QGridLayout());
	auto win = new WidgetMainWindow;
	wf->connectWidget(win);
	WidgetLoader* wl(new WidgetLoader());
	wl->linkLayoutGenerator(
			std::bind(&LayoutGenerator::changeWidget, this, std::placeholders::_1,
					  std::placeholders::_2),
			std::bind(&LayoutGenerator::changeLayout, this, std::placeholders::_1,
					  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
			wf->getWidgetTypes());
	wl->setLayoutHandle(grid);
	QWidget* wGrid = new QWidget();
	wGrid->setLayout(grid);
	grid->addWidget(wl);
	win->setCentralWidget(wGrid);
	win->show();
	win->resize(350, 100);
	windows_.emplace_back(win);
}

void
LayoutGenerator::addCustomWin()
{
	auto wf = get<GSWidgetFactory>();
	if (!wf)
	{
		CPSLOG_ERROR << "Widget factory missing";
		return;
	}
	QGridLayout* grid = new QGridLayout();
	QWidget* wGrid = new QWidget();
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			WidgetLoader* wl = new WidgetLoader(wGrid);
			wl->setLayoutHandle(grid);
			grid->addWidget(wl, x, y);
			wl->linkLayoutGenerator(
					std::bind(&LayoutGenerator::changeWidget, this, std::placeholders::_1,
							  std::placeholders::_2),
					std::bind(&LayoutGenerator::changeLayout, this, std::placeholders::_1,
							  std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
					wf->getWidgetTypes());
		}
	}

	wGrid->setLayout(grid);
	makeScrollableWin(wGrid);
}
