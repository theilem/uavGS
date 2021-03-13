set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverViewer/WidgetManeuverViewer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverViewer/GraphicsManeuverItem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PlanningManager.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverViewer/WidgetManeuverViewer.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/WidgetManeuverPlanner.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverViewer/WidgetManeuverViewer.h
        PARENT_SCOPE)
