#ifndef LAYOUTGENERATOR_H
#define LAYOUTGENERATOR_H

#include <QMainWindow>

#include <memory>

#include <cpsCore/cps_object>
#include "uavGS/LayoutGenerator/LayoutGeneratorParams.h"

class WidgetLoader;
class GSWidgetFactory;
class MapLogic;
class WidgetMainWindow;

/**
 * @brief   The LayoutGenerator class handles all the layouts and widgets in the
 *          ground station
 */
class LayoutGenerator: public AggregatableObject<GSWidgetFactory>,
		public IRunnableObject,
		public ConfigurableObject<LayoutParams>
{
public:

	friend class WidgetMainWindow;

	static constexpr TypeId typeId = "layout_generator";

	LayoutGenerator() = default;

	~LayoutGenerator();

	bool
	run(RunStage stage) override;

/**
 * @brief   addWidget is called when user clicks Add Widget from drop down menu
 */
void
addWidget();

private:

	/**
	 * @brief changeLayout replaces a WidgetLoader with specified layout
	 * @param wid pointer to WidgetLoader
	 * @param widget layout name to load
	 * @param rows number of rows or tabs
	 * @param cols number of columns
	 */
	void
	changeLayout(WidgetLoader* wid, const QString& type, int rows, int cols);

	/**
	 * @brief   changeWidget replaces a WidgetLoader with specified widget
	 * @param   wid pointer to WidgetLoader
	 * @param   widget name of widget to load
	 */
	void
	changeWidget(WidgetLoader* wid, const QString& widget);

	/**
	 * @brief   createLayout recursively parses a json defining the format of the
	 *          ground station. Returns a widget or format at each recursive call
	 * @param   json is the boost property representation of the json configuration
	 *          for the ground station
	 * @param   parent is the parent widget for the returned widget to be associated
	 *          with
	 * @return  QWidget pointer that represents the entire passed in json
	 */
	QWidget*
	createLayout(const LayoutParams& json, QWidget* parent);

	/**
	 * @brief makeScrollableWin creates a QMainWindow and makes it scrollable
	 * @param win pointer to QWidget to make center of window
	 */
	void
	makeScrollableWin(QWidget* win);

	///! vector of all windows currently displaying widgets
	std::vector<WidgetMainWindow*> windows_;

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
	 * @brief   addCustomWin is called when user clicks Create Custom Window from
	 *          drop down menu
	 */
	void
	addCustomWin();
};

#endif // LAYOUTGENERATOR_H
