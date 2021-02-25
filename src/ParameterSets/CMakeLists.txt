set(UAVGS_SRCS 	${UAVGS_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetGeneric.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/NamedCheckbox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/NamedLineEdit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LabeledValue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/NamedVectorEdit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetPopulator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetTreeNode.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetTreeParser.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetGeneric.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
        ${PROJECT_SOURCE_DIR}/include/uavGS/ParameterSets/WidgetGeneric.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ParameterSets/NamedCheckbox.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ParameterSets/NamedLineEdit.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ParameterSets/LabeledValue.h
        ${PROJECT_SOURCE_DIR}/include/uavGS/ParameterSets/NamedVectorEdit.h
        PARENT_SCOPE)
