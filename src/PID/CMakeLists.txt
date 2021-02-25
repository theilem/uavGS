set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/PIDConfigurator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/PIDConfigPlot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/PIDCustomPlot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetCPGrid.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetPIDPlots.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/PIDConfigPlot.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetCPGrid.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetPIDPlots.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/PID/Widgets/PIDConfigPlot.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/PID/Widgets/WidgetCPGrid.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/PID/Widgets/WidgetPIDPlots.h
        PARENT_SCOPE)

set(HEADERS_FOR_MOCS 	${HEADERS_FOR_MOCS}
        PARENT_SCOPE)