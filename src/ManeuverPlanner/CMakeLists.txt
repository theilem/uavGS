set(UAVGS_SRCS 	${UAVGS_SRCS}
		${CMAKE_CURRENT_SOURCE_DIR}/PlanningManager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverViewer/WidgetManeuverViewer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetSingletonManeuver.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetManeuverEditor.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionValue.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/Selection.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverViewer/WidgetManeuverViewer.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetSingletonManeuver.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetManeuverEditor.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionValue.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/Selection.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/WidgetManeuverPlanner.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverViewer/WidgetManeuverViewer.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/WidgetSingletonManeuver.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/WidgetManeuverEditor.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/SelectionValue.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/Selection.h
        PARENT_SCOPE)
