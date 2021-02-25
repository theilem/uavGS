set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetMainWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LayoutGenerator.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/WidgetLoader.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/LayoutGenerator/Widgets/WidgetLoader.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/LayoutGenerator/Widgets/WidgetMainWindow.h
        PARENT_SCOPE)
