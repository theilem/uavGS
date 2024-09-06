set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetNetworkStats.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/NamedDataPlot.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/Widgets/NamedDataPlot.h
        PARENT_SCOPE)
