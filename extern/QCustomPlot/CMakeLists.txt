# Wraspper CMakeLists to allow QCustomPlot to compile with CMake
# THIS FILE IS MADE BY THE AUTHORS OF uavGS AND IS NOT PROVIDED BY QCustomPlot

# Loosely adapted from https://doc.qt.io/qt-5/cmake-get-started.html

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

add_library(QCustomPlot QCustomPlot.cpp
		QCustomPlot.h)

target_link_libraries(QCustomPlot Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)