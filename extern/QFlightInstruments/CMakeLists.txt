# Wrapper CMakeLists to allow QFlightInstruments to compile with CMake
# THIS FILE IS MADE BY THE AUTHORS OF uavGS AND IS NOT PROVIDED BY QFlightInstruments

# Loosely adapted from https://doc.qt.io/qt-5/cmake-get-started.html

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Svg Widgets REQUIRED)

set(QFI_EX_SRC LayoutSquare.cpp
		WidgetADI.cpp
		WidgetALT.cpp
		WidgetASI.cpp
		WidgetHSI.cpp
		WidgetNAV.cpp
		WidgetPFD.cpp
		WidgetSix.cpp
		WidgetTC.cpp
		WidgetVSI.cpp)

set(QFI_EX_UI WidgetADI.ui
		WidgetALT.ui
		WidgetASI.ui
		WidgetHSI.ui
		WidgetNAV.ui
		WidgetPFD.ui
		WidgetSix.ui
		WidgetTC.ui
		WidgetVSI.ui)

include_directories(.)


add_library(QFlightInstruments SHARED qfi.qrc
		qfi_ADI.cpp
		qfi_ALT.cpp
		qfi_ASI.cpp
		qfi_HSI.cpp
		qfi_NAV.cpp
		qfi_PFD.cpp
		qfi_TC.cpp
		qfi_VSI.cpp
		${QFI_EX_SRC}
		${QFI_EX_UI})

target_link_libraries(QFlightInstruments Qt5::Svg Qt5::Widgets)

install(TARGETS QFlightInstruments
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
