cmake_minimum_required (VERSION 3.0.0)
project (uavGS)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -static-libstdc++ -std=c++1z -fPIC")



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Boost REQUIRED COMPONENTS system)

find_package(Qt5 #5.6
    COMPONENTS
        Core
        Gui
        OpenGL 
        Svg
        PrintSupport
    REQUIRED)
 
message(Qt5_VERSION ${Qt5_VERSION})
message(Qt5Core_VERSION ${Qt5Core_VERSION})
message(Qt5Gui_VERSION: ${Qt5Gui_VERSION})
message(Qt5OpenGL_VERSION: ${Qt5OpenGL_VERSION})
message(Qt5PrintSupport_VERSION: ${Qt5PrintSupport_VERSION})


set(SYSTEM_LIBS pthread
        dl)

if (NOT APPLE)
    set(SYSTEM_LIBS ${SYSTEM_LIBS} rt)
endif(NOT APPLE)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

set(SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/extern)


include_directories(${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/extern
  ${Qt5_INCLUDE_DIRS}
  ${Qt5Svg_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS})

add_subdirectory(extern/QCustomPlot)
add_subdirectory(extern/QFlightInstruments)

add_subdirectory(src)
#add_subdirectory(tests)

install(DIRECTORY include/uavGS DESTINATION include)
