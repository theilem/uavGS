set(PFD_SRCS)
set(PFD_FORMS)
set(PFD_HEADERS)

add_subdirectory(QFlightInstruments)

set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetSensorData.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SensorDataManager.cpp
        ${PFD_SRCS}
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetSensorData.ui
        ${PFD_FORMS}
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/WidgetSensorData.h
        ${PFD_HEADERS}
        PARENT_SCOPE)

set(HEADERS_FOR_MOCS 	${HEADERS_FOR_MOCS}
        PARENT_SCOPE)