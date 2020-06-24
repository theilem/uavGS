#ifndef WIDGETLOADER_H
#define WIDGETLOADER_H

#include <QWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <functional>

namespace Ui
{
class WidgetLoader;
}

class LayoutGenerator;

class WidgetLoader : public QWidget
{
Q_OBJECT

public:

	using ChangeWidget = std::function<void(WidgetLoader*, const QString&)>;

	using ChangeLayout = std::function<void(WidgetLoader*, const QString&, int, int)>;

	explicit
	WidgetLoader(QWidget* parent = 0);

	void
	linkLayoutGenerator(const ChangeWidget& changeWidget, const ChangeLayout& changeLayout,
						const std::vector<std::string>& widgets);

	void
	setLayoutHandle(QGridLayout* inLayout);

	QGridLayout*
	getLayoutHandle();

	void
	setTabHandle(QTabWidget* inTab);

	QTabWidget*
	getTabHandle();

	void
	setTabIndex(int index);

	int
	getTabIndex();

	~WidgetLoader();

public slots:

	void
	on_widgetButton_clicked();

	void
	on_layoutButton_clicked();

private:
	void
	widgetSelected(WidgetLoader* wid, const QString& widget);

	ChangeWidget changeWidget_;
	ChangeLayout changeLayout_;
	Ui::WidgetLoader* ui;
	QGridLayout* layout;
	QTabWidget* tab;
	int index;
};

#endif // WIDGETLOADER_H
