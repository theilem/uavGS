set(UAVGS_SRCS
		${CMAKE_CURRENT_SOURCE_DIR}/uavGS.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/GSWidgetFactory.cpp
        )

set(UI_FORMS)
set(WIDGET_HEADERS)
set(QT_RESOURCES
        ${PROJECT_SOURCE_DIR}/resources/qdarkstyle/style.qrc
        )

# When using autoRCC, creating a library with the resource and then linking it apparently doesn't work
set(EXTERNAL_RESOURCES ${PROJECT_SOURCE_DIR}/extern/QFlightInstruments/qfi.qrc)

add_subdirectory(PID)
add_subdirectory(MapLogic)
add_subdirectory(LayoutGenerator)
add_subdirectory(SensorData)
add_subdirectory(ParameterSets)
add_subdirectory(ManeuverPlanner)
add_subdirectory(PacketForwarding)

add_executable(uavGS ${UAVGS_SRCS}
		${UI_FORMS}
		${WIDGET_HEADERS}
		${QT_RESOURCES}
		${EXTERNAL_RESOURCES}
)

include_directories(${SUBMODULE_DIR}/Catch2/single_include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}/extern/QFlightInstruments)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(uavGS PRIVATE NODEBUG)
    message("Compiling with NODEBUG")
else ()

endif ()


target_link_libraries(uavGS
        ${SYSTEM_LIBS}
        Eigen3::Eigen
        ${Boost_LIBRARIES}
        Qt5::Widgets
        Qt5::Svg
        Qt5::OpenGL
        Qt5::PrintSupport
		${CPS_CORE_LIB}
        uavap_core
		QCustomPlot
		QFlightInstruments)

install(TARGETS uavGS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
