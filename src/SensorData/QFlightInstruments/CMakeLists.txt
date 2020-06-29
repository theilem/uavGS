set(PFD_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/LayoutSquare.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qfi_ADI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qfi_ALT.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qfi_ASI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qfi_HSI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qfi_PFD.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qfi_TC.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qfi_VSI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetADI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetALT.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetASI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetHSI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetPFD.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetSix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetTC.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetVSI.cpp
        PARENT_SCOPE)

set(PFD_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetADI.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetALT.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetASI.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetHSI.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetPFD.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetSix.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetTC.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetVSI.ui
        PARENT_SCOPE)

set(PFD_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/LayoutSquare.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/qfi_ADI.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/qfi_ALT.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/qfi_ASI.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/qfi_HSI.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/qfi_PFD.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/qfi_TC.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/qfi_VSI.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetADI.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetALT.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetASI.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetHSI.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetPFD.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetSix.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetTC.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFlightInstruments/WidgetVSI.h
        PARENT_SCOPE)
