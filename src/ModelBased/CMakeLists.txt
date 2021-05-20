set(UAVGS_SRCS 	${UAVGS_SRCS}
		${CMAKE_CURRENT_SOURCE_DIR}/ModelBasedHelperWidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelBasedConfigurators.cpp
		PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
		${CMAKE_CURRENT_SOURCE_DIR}/ModelBasedHelperWidget.ui
		PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
		${PROJECT_SOURCE_DIR}/include/uavGS/ModelBased/ModelBasedHelperWidget.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ModelBased/ModelBasedConfigurators.h
		PARENT_SCOPE)
