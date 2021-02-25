set(PFD_SRCS)
set(PFD_FORMS)
set(PFD_HEADERS)

add_subdirectory(QFI-Derived)

set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetSensorData.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SensorDataManager.cpp
        ${QFI_DERIVED_SRCS}
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetSensorData.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/WidgetSensorData.h
        ${QFI_DERIVED_HDRS}
        PARENT_SCOPE)
