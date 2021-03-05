set(QFI_DERIVED_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetPFD_GS.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/WidgetSix_GS.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/WidgetNAV_GS.cpp
        PARENT_SCOPE)

set(QFI_DERIVED_HDRS 	${UAVGS_SRCS}
		${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFI-Derived/WidgetPFD_GS.h
		${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFI-Derived/WidgetSix_GS.h
		${PROJECT_SOURCE_DIR}/include/uavGS/SensorData/QFI-Derived/WidgetNAV_GS.h
		PARENT_SCOPE)