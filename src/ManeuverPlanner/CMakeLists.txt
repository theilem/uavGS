set(UAVGS_SRCS 	${UAVGS_SRCS}
		${CMAKE_CURRENT_SOURCE_DIR}/PlanningManager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverViewer/WidgetManeuverViewer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/WidgetVisualScripter.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/WidgetMotionPrimitive.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/SelectionValue.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Selection.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Conditions/DurationCondition.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Conditions/SensorDataCondition.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Conditions/SteadyStateCondition.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Waveforms/WaveformWrapper.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Waveforms/SineWaveform.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Waveforms/SquareWaveform.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetSingletonManeuver.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetManeuverEditor.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionValue.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/Selection.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionObject.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/Reflection/ReflectionInterpreter.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionElement/BooleanElement.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionElement/FloatingElement.cpp
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionElement/EnumElement.cpp
        PARENT_SCOPE)

set(UI_FORMS 	${UI_FORMS}
        ${CMAKE_CURRENT_SOURCE_DIR}/WidgetManeuverPlanner.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverViewer/WidgetManeuverViewer.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/WidgetVisualScripter.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/WidgetMotionPrimitive.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/SelectionValue.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Selection.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Conditions/DurationCondition.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Conditions/SensorDataCondition.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Conditions/SteadyStateCondition.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Waveforms/WaveformWrapper.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Waveforms/SineWaveform.ui
		${CMAKE_CURRENT_SOURCE_DIR}/VisualScripter/Waveforms/SquareWaveform.ui
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetSingletonManeuver.ui
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/WidgetManeuverEditor.ui
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionValue.ui
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/Selection.ui
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionElement/BooleanElement.ui
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionElement/FloatingElement.ui
#		${CMAKE_CURRENT_SOURCE_DIR}/ManeuverEditor/SelectionElement/EnumElement.ui
        PARENT_SCOPE)

set(WIDGET_HEADERS 	${WIDGET_HEADERS}
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/WidgetManeuverPlanner.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverViewer/WidgetManeuverViewer.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/WidgetVisualScripter.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/WidgetMotionPrimitive.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/SelectionValue.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/Selection.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/Conditions/DurationCondition.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/Conditions/SensorDataCondition.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/Conditions/SteadyStateCondition.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/Waveforms/WaveformWrapper.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/Waveforms/SineWaveform.h
		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/VisualScripter/Waveforms/SquareWaveform.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/WidgetSingletonManeuver.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/WidgetManeuverEditor.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/SelectionValue.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/Selection.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/SelectionObject.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/BooleanElement.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/FloatingElement.h
#		${PROJECT_SOURCE_DIR}/include/uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/EnumElement.h
		PARENT_SCOPE)
