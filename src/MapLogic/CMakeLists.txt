set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/MapLogic.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MapLocation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/GraphicsMapView.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetOverheadMap.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetOverheadMap.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/MapLogic/Widgets/GraphicsMapView.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/MapLogic/Widgets/WidgetOverheadMap.h
        PARENT_SCOPE)
