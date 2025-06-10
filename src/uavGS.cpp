//
// Created by mirco on 17.06.20.
//

#include <boost/property_tree/json_parser.hpp>
#include <cpsCore/Synchronization/SimpleRunner.h>
#include "uavGS/GroundStationHelper.h"
#include <cpsCore/Configuration/JsonPopulator.h>
#include <QApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

int
main(int argc, char** argv)
{
	CPSLogger::instance()->setLogLevel(LogLevel::DEBUG);
	CPSLogger::instance()->setModuleName("uavGS");
	std::string configPath;
	if (argc == 2)
	{
		configPath = argv[1];
	}
	else
	{
		auto pop = JsonPopulator::populateContainer<GroundStationHelper>();
		std::cout << pop.getString() << std::endl;
		return 0;
	}
	QApplication app(argc, argv);

	Aggregator aggregator = GroundStationHelper::createAggregation(configPath);
	auto sched = aggregator.getOne<IScheduler>();
//	sched->setMainThread();

	auto lg = aggregator.getOne<LayoutGenerator>();

	QFile f(QString::fromStdString(lg->getResourcePath() + "qdarkstyle/style.qss"));
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

	auto sh = aggregator.getOne<SignalHandler>();
	sh->subscribeOnSigint(std::bind(QApplication::quit));

	CPSLOG_DEBUG << "Run synchronized";
	SimpleRunner runner(aggregator);
	if (runner.runAllStages())
	{
		CPSLOG_ERROR << "Something went wrong";
		return 1;
	}

	app.exec();

	//Terminated -> Cleanup
	aggregator.cleanUp();

	return 0;
}
