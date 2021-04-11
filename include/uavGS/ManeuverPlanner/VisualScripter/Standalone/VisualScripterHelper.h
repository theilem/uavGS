//
// Created by seedship on 4/9/21.
//

#include <cpsCore/Framework/StaticHelper.h>
#include <uavGS/GSWidgetFactory.h>
#include "uavGS/ManeuverPlanner/VisualScripter/Standalone/StandaloneOverrideProvider.h"

#ifndef UAVGS_VISUALSCRIPTERHELPER_H
#define UAVGS_VISUALSCRIPTERHELPER_H

using VisualScripterDefaults = StaticHelper<GSWidgetFactory, StandaloneOverrideProvider>;

using VisualScripterHelper = StaticHelper<VisualScripterDefaults>;

#endif //UAVGS_VISUALSCRIPTERHELPER_H
