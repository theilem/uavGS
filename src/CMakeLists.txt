set(UAVGS_SRCS
        GSWidgetFactory.cpp
        )

set(UI_FORMS)
set(WIDGET_HEADERS)
set(HEADERS_FOR_MOCS)
set(QT_RESOURCES
        ${PROJECT_SOURCE_DIR}/resources/qdarkstyle/style.qrc
        ${PROJECT_SOURCE_DIR}/resources/QFlightInstruments/qfi.qrc
        )

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
		${HEADERS_FOR_MOCS}
		${QT_RESOURCES}
        uavGS.cpp)

include_directories(${SUBMODULE_DIR}/Catch2/single_include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/src)

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
		cps_core
        uavap_core
		QCustomPlot)

install(TARGETS uavGS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
