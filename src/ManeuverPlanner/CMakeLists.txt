set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PlanningManager.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverEditor.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/WidgetManeuverPlanner.h
        PARENT_SCOPE)

set(HEADERS_FOR_MOCS 	${HEADERS_FOR_MOCS}
        PARENT_SCOPE)