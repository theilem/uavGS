//
// Created by mirco on 19.06.20.
//

#ifndef UAVGS_MAPLOGICPARAMS_H
#define UAVGS_MAPLOGICPARAMS_H

#include <cpsCore/Configuration/Parameter.hpp>
#include "uavGS/MapLogic/MapLocation.h"

struct MapLogicParams
{
	Parameter<unsigned> flightPathSize = {600, "flight_path_size", false};
	Parameter<MapLocationParams> mapCenter = {{}, "map_center", true};

	template<typename Config>
	inline void
	configure(Config& c)
	{
		c & flightPathSize;
		c & mapCenter;
	}

};


#endif //UAVGS_MAPLOGICPARAMS_H
