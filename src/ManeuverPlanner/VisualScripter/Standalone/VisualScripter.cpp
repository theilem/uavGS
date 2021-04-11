//
// Created by seedship on 4/9/21.
//

#include <cpsCore/Synchronization/SimpleRunner.h>
#include <QFileDialog>
#include <cpsCore/Logging/CPSLogger.h>
#include <uavGS/ManeuverPlanner/VisualScripter/Standalone/VisualScripterHelper.h>
#include <QTextStream>
#include <QApplication>
#include <uavGS/ManeuverPlanner/VisualScripter/WidgetVisualScripter.h>

int
main(int argc, char** argv)
{
	CPSLogger::instance()->setLogLevel(LogLevel::DEBUG);
	CPSLogger::instance()->setModuleName("VisualScripter");
	std::string configPath;

	QApplication app(argc, argv);


	QFile f(QString::fromStdString("/usr/local/resources/uavGS/qdarkstyle/style.qss"));
	if (f.exists())
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		app.setStyleSheet(ts.readAll());
		CPSLOG_TRACE << "Loaded custom stylesheet.";
	}
	else
	{
		CPSLOG_WARN << "Could not open resource file at " << f.symLinkTarget().toStdString();
	}

	if (argc == 2)
	{
		configPath = argv[1];
	}
	else
	{
		QFileDialog dialog;
		dialog.setWindowTitle("Select Flight Control Configuration");
		dialog.setFileMode(QFileDialog::ExistingFile);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		if (dialog.exec())
		{
			configPath = dialog.selectedFiles().front().toStdString();
		}
		else
		{
			CPSLOG_TRACE << "Cancelled config loading.";
			return -1;
		}
	}

	auto agg = VisualScripterHelper::createAggregation();
	auto op = agg.getOne<StandaloneOverrideProvider>();
	op->loadConfig(configPath);

	SimpleRunner runner(agg);
	if (runner.runAllStages())
	{
		CPSLOG_ERROR << "Something went wrong";
		return 1;
	}

	auto wf = agg.getOne<GSWidgetFactory>();
	wf->registerWidget<WidgetVisualScripter>();

	auto sh = agg.getOne<SignalHandler>();
	sh->subscribeOnSigint(std::bind(QApplication::quit));

	QMainWindow window;
	window.setWindowTitle("Visual Scripter");

	auto w = wf->createWidget("visual_scripter", &window);
	window.setCentralWidget(w);
	window.show();
	app.exec();

	agg.cleanUp();

	return 0;
}