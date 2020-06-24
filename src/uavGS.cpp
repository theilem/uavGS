//
// Created by mirco on 17.06.20.
//

#include <boost/property_tree/json_parser.hpp>
#include <cpsCore/Synchronization/SimpleRunner.h>
#include "uavGS/GroundStationHelper.h"
#include <cpsCore/Configuration/JsonPopulator.h>
#include <QApplication>

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
		std::ofstream file;
		file.open("generate.json", std::ofstream::out);
		JsonPopulator pop(file);

		pop.populateContainer(GroundStationHelper());
		std::cout << "Populated json" << std::endl;
		return 0;
	}
	QApplication app(argc, argv);

	Aggregator aggregator = GroundStationHelper::createAggregation(configPath);
	auto sched = aggregator.getOne<IScheduler>();
//	sched->setMainThread();

	auto sh = aggregator.getOne<SignalHandler>();
	sh->subscribeOnSigint(std::bind(QApplication::quit));

	CPSLOG_DEBUG << "Run synchronized";
	SimpleRunner runner(aggregator);
	if (runner.runAllStages())
	{
		CPSLOG_ERROR << "Something went wrong";
		return 1;
	}

	sched->startSchedule();

	app.exec();

	//Terminated -> Cleanup
	aggregator.cleanUp();

	return 0;
}
