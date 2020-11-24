# Wrapper CMakeLists to allow QFlightInstruments to compile with CMake
# THIS FILE IS MADE BY THE AUTHORS OF uavGS AND IS NOT PROVIDED BY QFlightInstruments

# Loosely adapted from https://doc.qt.io/qt-5/cmake-get-started.html

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Svg Widgets REQUIRED)

set(QFI_EX_SRC example/LayoutSquare.cpp
		example/WidgetADI.cpp
		example/WidgetALT.cpp
		example/WidgetASI.cpp
		example/WidgetHSI.cpp
		example/WidgetNAV.cpp
		example/WidgetPFD.cpp
		example/WidgetSix.cpp
		example/WidgetTC.cpp
		example/WidgetVSI.cpp)

set(QFI_EX_UI example/WidgetADI.ui
		example/WidgetALT.ui
		example/WidgetASI.ui
		example/WidgetHSI.ui
		example/WidgetNAV.ui
		example/WidgetPFD.ui
		example/WidgetSix.ui
		example/WidgetTC.ui
		example/WidgetVSI.ui)

# In the QFI project, both directories are included https://github.com/marek-cel/QFlightinstruments/blob/master/src/example.pro#L23
include_directories(.)
include_directories(example)


add_library(QFlightInstruments qfi.qrc
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