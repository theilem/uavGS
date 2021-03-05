/**
 *   @author Richard Nai, rnai2@illinois.edu
 *   @file IDataSignals.h
 *   @date [dd/mm/yyyy] 3/21/2018
 *   @brief UAV Ground Station PID Configuration Interface
 */

#ifndef IPIDCONFIGURATOR_H
#define IPIDCONFIGURATOR_H

#include "uavAP/FlightControl/Controller/PIDController/PIDHandling.h"
#include "uavAP/Core/DataHandling/DataHandling.h"
#include <cpsCore/cps_object>
#include <QtCore/QObject>
//#include "uavGS/GSWidgetFactory.h"
class GSWidgetFactory;
/**
 * @brief   The IPIDConfigurator class serves as an interface for all classes
 *          that have PID tuning functionality.
 */
class PIDConfigurator : public AggregatableObject<DataHandling, GSWidgetFactory>, public IRunnableObject
{
public:

	static constexpr TypeId typeId = "pid_configurator";

	/**
	 * @brief   tunePID tunes a certain PID with Kp, kI, kD, Imax, and ff gains
	 * @param   tunePID PIDTuning struct containing integer PID controller ID and gains
	 * @return  true if request was recieved
	 */
	bool
	tunePID(const PIDTuning& tunePID);

	/**
	 * @brief   getPIDMap gets PID map of flying aircraft's cascade
	 * @return  map from integer PID representation to PIDInfo struct containing
	 *          human readable name and PID parameters
	 */
	const PIDParams&
	getPIDMap() const;

	void
	requestPIDParams();

	bool
	run(RunStage stage) override;

private:

	PIDParams pidParams_;

};

#endif // IPIDCONFIGURATOR_H
