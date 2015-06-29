CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PearSim-Lib)

set(PSL_Root ${PS_Root}/lib)

#CONFIGURE
configure_file(${PSL_Root}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

SET(PSL_MAIN_Src)
SOURCE_GROUP("Source Files" FILES ${PSL_MAIN_Src})
SET(PSL_ACTORS_Src
  ${PSL_Root}/actors/emptyactor.cpp
  ${PSL_Root}/actors/grid.cpp
  ${PSL_Root}/actors/gridbox.cpp
  ${PSL_Root}/actors/iactor.cpp
  ${PSL_Root}/actors/mesh.cpp
  ${PSL_Root}/actors/rectangular.cpp
  ${PSL_Root}/actors/sphere.cpp)
SOURCE_GROUP("Source Files\\Actors" FILES ${PSL_ACTORS_Src})
SET(PSL_INTERACTORS_Src
  ${PSL_Root}/interactors/actorinteractor.cpp
  ${PSL_Root}/interactors/camerainteractor.cpp
  ${PSL_Root}/interactors/iinteractor.cpp)
SOURCE_GROUP("Source Files\\Interactors" FILES ${PSL_INTERACTORS_Src})
SET(PSL_LOADER_Src
  ${PSL_Root}/loader/wavefrontloader.cpp)
SOURCE_GROUP("Source Files\\Loader" FILES ${PSL_LOADER_Src})
SET(PSL_PLUGIN_Src
  ${PSL_Root}/plugin/plugin.cpp
  ${PSL_Root}/plugin/pluginmanager.cpp)
SOURCE_GROUP("Source Files\\Plugin" FILES ${PSL_PLUGIN_Src})
SET(PSL_PROPERTIES_Src
  ${PSL_Root}/properties/colorproperty.cpp
  ${PSL_Root}/properties/doubleproperty.cpp
  ${PSL_Root}/properties/groupproperty.cpp
  ${PSL_Root}/properties/intproperty.cpp
  ${PSL_Root}/properties/iproperty.cpp
  ${PSL_Root}/properties/propertytable.cpp
  ${PSL_Root}/properties/selectionproperty.cpp
  ${PSL_Root}/properties/textproperty.cpp)
SOURCE_GROUP("Source Files\\Properties" FILES ${PSL_PROPERTIES_Src})
SET(PSL_RENDERER_Src
  ${PSL_Root}/renderer/camera.cpp
  ${PSL_Root}/renderer/environment.cpp
  ${PSL_Root}/renderer/light.cpp
  ${PSL_Root}/renderer/material.cpp
  ${PSL_Root}/renderer/renderer.cpp
  ${PSL_Root}/renderer/shader.cpp)
SOURCE_GROUP("Source Files\\Renderer" FILES ${PSL_RENDERER_Src})
SET(PSL_SIMS_Src
  ${PSL_Root}/sims/isimulation.cpp)
SOURCE_GROUP("Source Files\\Sims" FILES ${PSL_SIMS_Src})
SET(PSL_UTILS_Src
  ${PSL_Root}/utils/colorbutton.cpp
  ${PSL_Root}/utils/gradient.cpp)
SOURCE_GROUP("Source Files\\Utils" FILES ${PSL_UTILS_Src})
SET(PSL_Src
  ${PSL_MAIN_Src} ${PSL_ACTORS_Src} ${PSL_INTERACTORS_Src}
  ${PSL_LOADER_Src} ${PSL_PROPERTIES_Src} ${PSL_RENDERER_Src}
  ${PSL_SIMS_Src} ${PSL_UTILS_Src} ${PSL_PLUGIN_Src}
)

SET(PSL_MAIN_Hdr
  ${PSL_Root}/config.h.in)
SOURCE_GROUP("Header Files" FILES ${PSL_MAIN_Hdr})
SET(PSL_ACTORS_Hdr
  ${PSL_Root}/actors/emptyactor.h
  ${PSL_Root}/actors/grid.h
  ${PSL_Root}/actors/gridbox.h
  ${PSL_Root}/actors/iactor.h
  ${PSL_Root}/actors/mesh.h
  ${PSL_Root}/actors/rectangular.h
  ${PSL_Root}/actors/sphere.h)
SOURCE_GROUP("Header Files\\Actors" FILES ${PSL_ACTORS_Hdr})
SET(PSL_DATA_Hdr
  ${PSL_Root}/data/data.h)
SOURCE_GROUP("Header Files\\Data" FILES ${PSL_DATA_Hdr})
SET(PSL_INTERACTORS_Hdr
  ${PSL_Root}/interactors/actorinteractor.h
  ${PSL_Root}/interactors/camerainteractor.h
  ${PSL_Root}/interactors/iinteractor.h)
SOURCE_GROUP("Header Files\\Interactors" FILES ${PSL_INTERACTORS_Hdr})
SET(PSL_LOADER_Hdr
  ${PSL_Root}/loader/meshloader.h
  ${PSL_Root}/loader/wavefrontloader.h)
SOURCE_GROUP("Header Files\\Loader" FILES ${PSL_LOADER_Hdr})
SET(PSL_PLUGIN_Hdr
  ${PSL_Root}/plugin/iplugin.h
  ${PSL_Root}/plugin/plugin.h
  ${PSL_Root}/plugin/pluginmanager.h)
SOURCE_GROUP("Header Files\\Plugin" FILES ${PSL_PLUGIN_Hdr})
SET(PSL_PROPERTIES_Hdr
  ${PSL_Root}/properties/colorproperty.h
  ${PSL_Root}/properties/doubleproperty.h
  ${PSL_Root}/properties/groupproperty.h
  ${PSL_Root}/properties/intproperty.h
  ${PSL_Root}/properties/iproperty.h
  ${PSL_Root}/properties/propertytable.h
  ${PSL_Root}/properties/selectionproperty.h
  ${PSL_Root}/properties/textproperty.h)
SOURCE_GROUP("Header Files\\Properties" FILES ${PSL_PROPERTIES_Hdr})
SET(PSL_RENDERER_Hdr
  ${PSL_Root}/renderer/camera.h
  ${PSL_Root}/renderer/environment.h
  ${PSL_Root}/renderer/light.h
  ${PSL_Root}/renderer/material.h
  ${PSL_Root}/renderer/renderer.h
  ${PSL_Root}/renderer/shader.h)
SOURCE_GROUP("Header Files\\Renderer" FILES ${PSL_RENDERER_Hdr})
SET(PSL_SIMS_Hdr
  ${PSL_Root}/sims/isimulation.h)
SOURCE_GROUP("Header Files\\Sims" FILES ${PSL_SIMS_Hdr})
SET(PSL_UTILS_Hdr
  ${PSL_Root}/utils/colorbutton.h
  ${PSL_Root}/utils/gradient.h)
SOURCE_GROUP("Header Files\\Utils" FILES ${PSL_UTILS_Hdr})
SET(PSL_Hdr
  ${PSL_MAIN_Hdr} ${PSL_ACTORS_Hdr} ${PSL_DATA_Hdr} ${PSL_INTERACTORS_Hdr}
  ${PSL_LOADER_Hdr} ${PSL_PROPERTIES_Hdr} ${PSL_RENDERER_Hdr}
  ${PSL_SIMS_Hdr} ${PSL_UTILS_Hdr} ${PSL_PLUGIN_Hdr}
)

QT5_WRAP_CPP(PSL_Hdr_Moc ${PSL_Hdr})

SOURCE_GROUP("Generated Files" FILES ${PSL_Hdr_Moc})

add_library(PearSimLib SHARED ${PSL_Src} ${PSL_Hdr} ${PSL_Hdr_Moc})
target_link_libraries(PearSimLib Qt5::Widgets)
target_link_libraries(PearSimLib Qt5::OpenGL)
target_link_libraries(PearSimLib ${OPENGL_LIBRARIES})
target_link_libraries(PearSimLib ${OpenCL_LIBRARIES})
set_target_properties(PearSimLib PROPERTIES VERSION ${PS_Version})
set_target_properties(PearSimLib PROPERTIES COMPILE_DEFINITIONS "${DEFS};PS_LIB_BUILD" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")