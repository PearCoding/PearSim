CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PearSim-Main)

set(PSM_Root ${PS_Root}/main)

SET(PSM_MAIN_Src
  ${PSM_Root}/main.cpp)
SOURCE_GROUP("Source Files" FILES ${PSM_MAIN_Src})
SET(PSM_WIDGETS_Src
  ${PSM_Root}/widgets/mainwindow.cpp
  ${PSM_Root}/widgets/propertyview.cpp
  ${PSM_Root}/widgets/simulationview.cpp)
SOURCE_GROUP("Source Files\\Widgets" FILES ${PSM_WIDGETS_Src})
SET(PSM_Src
  ${PSM_MAIN_Src} ${PSM_WIDGETS_Src}
)

SET(PSM_MAIN_Hdr)
SOURCE_GROUP("Header Files" FILES ${PSM_MAIN_Hdr})
SET(PSM_WIDGETS_Hdr
  ${PSM_Root}/widgets/mainwindow.h
  ${PSM_Root}/widgets/propertyview.h
  ${PSM_Root}/widgets/simulationview.h)
SOURCE_GROUP("Header Files\\Widgets" FILES ${PSM_WIDGETS_Hdr})
SET(PSM_Hdr
  ${PSM_MAIN_Hdr} ${PSM_WIDGETS_Hdr}
)

SET(PSM_Fms
  ${PSM_Root}/ui/mainwindow.ui)
SOURCE_GROUP("Form Files" FILES ${PSM_Fms})

QT5_WRAP_CPP(PSM_Hdr_Moc ${PSM_Hdr})
QT5_WRAP_UI(PSM_Fms_Hdr ${PSM_Fms})

SOURCE_GROUP("Generated Files" FILES ${PSM_Hdr_Moc} ${PSM_Fms_Hdr})

add_executable(${PS_TARGET} ${PSM_Src} ${PSM_Hdr}
		 ${PSM_Hdr_Moc} ${PSM_Fms_Hdr})
target_link_libraries(${PS_TARGET} PearSimLib)
target_link_libraries(${PS_TARGET} Qt5::Widgets)
target_link_libraries(${PS_TARGET} Qt5::OpenGL)
target_link_libraries(${PS_TARGET} ${OPENGL_LIBRARIES})
target_link_libraries(${PS_TARGET} ${OpenCL_LIBRARIES})
set_target_properties(${PS_TARGET} PROPERTIES VERSION ${PS_Version})
set_target_properties(${PS_TARGET} PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")