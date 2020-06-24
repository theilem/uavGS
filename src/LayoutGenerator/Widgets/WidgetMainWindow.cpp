//
// Created by mirco on 23.06.20.
//

#include "uavGS/LayoutGenerator/Widgets/WidgetMainWindow.h"
#include <QAction>
#include <QMenuBar>

WidgetMainWindow::WidgetMainWindow()
{
}

void
WidgetMainWindow::addMenus()
{
	//it might be a problem that we are creating the actions with nullptr parents,
	//however, when I try linking them to win, it sigabrts on exit
	QMenu* fileMenu = this->menuBar()->addMenu("File");
	QAction* quitAct = new QAction("Quit", nullptr);
	quitAct->setShortcut(QKeySequence::Quit);
	QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(handleQuit())); 
	QAction* addWindow = new QAction("Add Window from Config", nullptr);
	QObject::connect(addWindow, SIGNAL(triggered()), this, SLOT(addWin()));
	addWindow->setShortcut(QKeySequence::New);
	QAction* addCustomWindow = new QAction("Create Custom Window", nullptr);
	QObject::connect(addCustomWindow, SIGNAL(triggered()), this, SLOT(addCustomWin()));
	addCustomWindow->setShortcut(QKeySequence::AddTab);
	QAction* addWidget = new QAction("Add Widget", nullptr);
	addWidget->setShortcut(tr("Ctrl+Shift+N"));
	QObject::connect(addWidget, SIGNAL(triggered()), this, SLOT(addWidget()));
	fileMenu->addAction(addWidget);
	fileMenu->addAction(addWindow);
	fileMenu->addAction(addCustomWindow);
	fileMenu->addAction(quitAct);
}

void
WidgetMainWindow::connect()
{
	addMenus();
}

void
WidgetMainWindow::handleQuit()
{
	if (auto lg = get<LayoutGenerator>())
	{
		lg->handleQuit();
	}
}

void
WidgetMainWindow::addWin()
{
	if (auto lg = get<LayoutGenerator>())
	{
		lg->addWin();
	}

}

void
WidgetMainWindow::addWidget()
{

	if (auto lg = get<LayoutGenerator>())
	{
		lg->addWidget();
	}
}

void
WidgetMainWindow::addCustomWin()
{

}
