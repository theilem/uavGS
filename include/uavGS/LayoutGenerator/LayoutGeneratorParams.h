//
// Created by mirco on 23.06.20.
//

#ifndef UAVGS_LAYOUTGENERATORPARAMS_H
#define UAVGS_LAYOUTGENERATORPARAMS_H

#include <cpsCore/Configuration/Parameter.hpp>
#include <cpsCore/Utilities/EnumMap.hpp>
#include <vector>

enum class Layouts
{
	HORIZONTAL,
	VERTICAL,
	QUAD,
	TAB
};

ENUMMAP_INIT(Layouts,
			 {
				 { Layouts::HORIZONTAL, "horizontal" },
				 { Layouts::VERTICAL, "vertical" },
				 { Layouts::QUAD, "quad" },
				 { Layouts::TAB, "tab" },
			 });


struct LayoutParams
{
	Parameter<Layouts> layout = {Layouts::HORIZONTAL, "layout", false};
	Parameter<std::vector<LayoutParams>> items = {{}, "items", false};
	Parameter<std::string> widget = {"", "widget", false};

	template<typename Config>
	inline void
	configure(Config& c)
	{
		c & layout;
		c & items;
		c & widget;
	}
};

//struct LayoutGeneratorParams
//{
//
//};


#endif //UAVGS_LAYOUTGENERATORPARAMS_H
