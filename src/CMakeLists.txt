CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PearSim-Main)

#CONFIGURE
configure_file(${PS_Root}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

SET(PS_MAIN_Src
  ${PS_Root}/main.cpp)
SOURCE_GROUP("Source Files" FILES ${WS_MAIN_Src})
SET(PS_ACTORS_Src
  ${PS_Root}/actors/emptyactor.cpp
  ${PS_Root}/actors/grid.cpp
  ${PS_Root}/actors/gridbox.cpp
  ${PS_Root}/actors/iactor.cpp
  ${PS_Root}/actors/mesh.cpp
  ${PS_Root}/actors/rectangular.cpp
  ${PS_Root}/actors/sphere.cpp)
SOURCE_GROUP("Source Files\\Actors" FILES ${PS_ACTORS_Src})
SET(PS_INTERACTORS_Src
  ${PS_Root}/interactors/actorinteractor.cpp
  ${PS_Root}/interactors/camerainteractor.cpp
  ${PS_Root}/interactors/iinteractor.cpp)
SOURCE_GROUP("Source Files\\Interactors" FILES ${PS_INTERACTORS_Src})
SET(PS_LOADER_Src
  ${PS_Root}/loader/wavefrontloader.cpp)
SOURCE_GROUP("Source Files\\Loader" FILES ${PS_LOADER_Src})
SET(PS_PROPERTIES_Src
  ${PS_Root}/properties/ipropertytable.cpp)
SOURCE_GROUP("Source Files\\Properties" FILES ${PS_PROPERTIES_Src})
SET(PS_RENDERER_Src
  ${PS_Root}/renderer/camera.cpp
  ${PS_Root}/renderer/environment.cpp
  ${PS_Root}/renderer/light.cpp
  ${PS_Root}/renderer/material.cpp
  ${PS_Root}/renderer/renderer.cpp
  ${PS_Root}/renderer/shader.cpp)
SOURCE_GROUP("Source Files\\Renderer" FILES ${PS_RENDERER_Src})
SET(PS_SIMS_Src
  ${PS_Root}/sims/blocksimulation.cpp
  ${PS_Root}/sims/isimulation.cpp
  ${PS_Root}/sims/poisson3d.cpp
  ${PS_Root}/sims/springsimulation.cpp
  ${PS_Root}/sims/watersimulation.cpp)
SOURCE_GROUP("Source Files\\Sims" FILES ${PS_SIMS_Src})
SET(PS_UTILS_Src
  ${PS_Root}/utils/gradient.cpp)
SOURCE_GROUP("Source Files\\Utils" FILES ${PS_UTILS_Src})
SET(PS_WIDGETS_Src
  ${PS_Root}/widgets/mainwindow.cpp
  ${PS_Root}/widgets/simulationview.cpp)
SOURCE_GROUP("Source Files\\Widgets" FILES ${PS_WIDGETS_Src})
SET(PS_Src
  ${PS_MAIN_Src} ${PS_ACTORS_Src} ${PS_INTERACTORS_Src}
  ${PS_LOADER_Src} ${PS_PROPERTIES_Src} ${PS_RENDERER_Src}
  ${PS_SIMS_Src} ${PS_UTILS_Src} ${PS_WIDGETS_Src}
)

SET(PS_MAIN_Hdr
  ${PS_Root}/config.h.in)
SOURCE_GROUP("Header Files" FILES ${PS_MAIN_Hdr})
SET(PS_ACTORS_Hdr
  ${PS_Root}/actors/emptyactor.h
  ${PS_Root}/actors/grid.h
  ${PS_Root}/actors/gridbox.h
  ${PS_Root}/actors/iactor.h
  ${PS_Root}/actors/mesh.h
  ${PS_Root}/actors/rectangular.h
  ${PS_Root}/actors/sphere.h)
SOURCE_GROUP("Header Files\\Actors" FILES ${PS_ACTORS_Hdr})
SET(PS_DATA_Hdr
  ${PS_Root}/data/datacontainer.h
  ${PS_Root}/data/datagrid.h)
SOURCE_GROUP("Header Files\\Data" FILES ${PS_DATA_Hdr})
SET(PS_INTERACTORS_Hdr
  ${PS_Root}/interactors/actorinteractor.h
  ${PS_Root}/interactors/camerainteractor.h
  ${PS_Root}/interactors/iinteractor.h)
SOURCE_GROUP("Header Files\\Interactors" FILES ${PS_INTERACTORS_Hdr})
SET(PS_LOADER_Hdr
  ${PS_Root}/loader/meshloader.h
  ${PS_Root}/loader/wavefrontloader.h)
SOURCE_GROUP("Header Files\\Loader" FILES ${PS_LOADER_Hdr})
SET(PS_PROPERTIES_Hdr
  ${PS_Root}/properties/ipropertytable.h)
SOURCE_GROUP("Header Files\\Properties" FILES ${PS_PROPERTIES_Hdr})
SET(PS_RENDERER_Hdr
  ${PS_Root}/renderer/camera.h
  ${PS_Root}/renderer/environment.h
  ${PS_Root}/renderer/light.h
  ${PS_Root}/renderer/material.h
  ${PS_Root}/renderer/renderer.h
  ${PS_Root}/renderer/shader.h)
SOURCE_GROUP("Header Files\\Renderer" FILES ${PS_RENDERER_Hdr})
SET(PS_SIMS_Hdr
  ${PS_Root}/sims/blocksimulation.h
  ${PS_Root}/sims/isimulation.h
  ${PS_Root}/sims/poisson3d.h
  ${PS_Root}/sims/springsimulation.h
  ${PS_Root}/sims/watersimulation.h)
SOURCE_GROUP("Header Files\\Sims" FILES ${PS_SIMS_Hdr})
SET(PS_UTILS_Hdr
  ${PS_Root}/utils/gradient.h)
SOURCE_GROUP("Header Files\\Utils" FILES ${PS_UTILS_Hdr})
SET(PS_WIDGETS_Hdr
  ${PS_Root}/widgets/mainwindow.h
  ${PS_Root}/widgets/simulationview.h)
SOURCE_GROUP("Header Files\\Widgets" FILES ${PS_WIDGETS_Hdr})
SET(PS_Hdr
  ${PS_MAIN_Hdr} ${PS_ACTORS_Hdr} ${PS_Data_Hdr} ${PS_INTERACTORS_Hdr}
  ${PS_LOADER_Hdr} ${PS_PROPERTIES_Hdr} ${PS_RENDERER_Hdr}
  ${PS_SIMS_Hdr} ${PS_UTILS_Hdr} ${PS_WIDGETS_Hdr}
)

SET(PS_Fms
  ${PS_Root}/ui/mainwindow.ui)
SOURCE_GROUP("Form Files" FILES ${PS_Fms})

QT5_WRAP_CPP(PS_Hdr_Moc ${PS_Hdr})
QT5_WRAP_UI(PS_Fms_Hdr ${PS_Fms})

SOURCE_GROUP("Generated Files" FILES ${PS_Hdr_Moc} ${PS_Fms_Hdr})

add_executable(${PS_TARGET} ${PS_Src} ${PS_Hdr}
		 ${PS_Hdr_Moc} ${PS_Fms_Hdr})
target_link_libraries(${PS_TARGET} Qt5::Widgets)
target_link_libraries(${PS_TARGET} Qt5::OpenGL)
target_link_libraries(${PS_TARGET} ${OPENGL_LIBRARIES})
set_target_properties(${PS_TARGET} PROPERTIES VERSION ${PS_Version})
set_target_properties(${PS_TARGET} PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")