//
// Created by mirco on 23.06.20.
//

#ifndef UAVGS_WIDGETMAINWINDOW_H
#define UAVGS_WIDGETMAINWINDOW_H

#include <QMainWindow>
#include <cpsCore/Aggregation/AggregatableObject.hpp>
#include <uavGS/LayoutGenerator/LayoutGenerator.h>

class WidgetMainWindow : public QMainWindow, public AggregatableObject<LayoutGenerator>
{
Q_OBJECT
public:
	WidgetMainWindow();

	/**
	 * @brief   addMenus adds top menu to the specified window
	 * @param   win pointer to menu to add menu too
	 */
	void
	addMenus();

	void
	connect();

private slots:

	/**
 * @brief   handleQuit is called when user clicks Quit from drop down menu
 */
	void
	handleQuit();

	/**
	 * @brief   addWin is called when user clicks Add Window from Config from drop
	 *          down menu
	 */
	void
	addWin();

	/**
	 * @brief   addWidget is called when user clicks Add Widget from drop down menu
	 */
	void
	addWidget();

	/**
	 * @brief   addCustomWin is called when user clicks Create Custom Window from
	 *          drop down menu
	 */
	void
	addCustomWin();

};


#endif //UAVGS_WIDGETMAINWINDOW_H
