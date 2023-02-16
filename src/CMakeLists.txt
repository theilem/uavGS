set(UAVGS_SRCS
        GSWidgetFactory.cpp
        )

set(UI_FORMS)
set(WIDGET_HEADERS)
set(QT_RESOURCES
        ${PROJECT_SOURCE_DIR}/resources/qdarkstyle/style.qrc
        )

set(VISUALSCRIPT_SRC)
set(VISUALSCRIPT_UI)
set(VISUALSCRIPT_H)

# When using autoRCC, creating a library with the resource and then linking it apparently doesn't work
set(EXTERNAL_RESOURCES ${CMAKE_SOURCE_DIR}/extern/QFlightInstruments/qfi.qrc)

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
		${VISUALSCRIPT_SRC}
		${VISUALSCRIPT_UI}
		${VISUALSCRIPT_H}
		${QT_RESOURCES}
		${EXTERNAL_RESOURCES}
        uavGS.cpp)

#
add_executable(VisualScripter
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverPlanner/VisualScripter/Standalone/VisualScripter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverPlanner/VisualScripter/Standalone/StandaloneOverrideProvider.cpp
		# Note: I think this makes the visual scripting widget double compiled, but I feel like it is too big
		# to make into a library
		${VISUALSCRIPT_SRC}
		${VISUALSCRIPT_UI}
		${VISUALSCRIPT_H}
		${QT_RESOURCES}
		${EXTERNAL_RESOURCES}
		# Need this to be able to add widgets to aggregation
		GSWidgetFactory.cpp
		)


include_directories(${SUBMODULE_DIR}/Catch2/single_include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/src
		${CMAKE_SOURCE_DIR}/extern/QFlightInstruments)

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
		QCustomPlot
		QFlightInstruments)


target_link_libraries(VisualScripter
		${SYSTEM_LIBS}
		Eigen3::Eigen
		${Boost_LIBRARIES}
		Qt5::Widgets
		Qt5::Svg
		Qt5::OpenGL
		Qt5::PrintSupport
		cps_core
		uavap_flight_control
		uavap_core)

install(TARGETS uavGS
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
