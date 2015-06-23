CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PS-Plugin-Water)

SET(_HDR ${PSP_Root}/water/watersimulation.h)
QT5_WRAP_CPP(_HDR_Moc ${_HDR})

add_library(ps_water_plugin MODULE ${PSP_Root}/water/watersimulation.cpp ${_HDR} ${_HDR_Moc})
target_link_libraries(ps_water_plugin PearSimLib)
target_link_libraries(ps_water_plugin Qt5::Widgets)
set_target_properties(ps_water_plugin PROPERTIES VERSION ${PS_Version})
set_target_properties(ps_water_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
set_target_properties(ps_water_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PS_PLUGIN_OUTPUT_PATH}")
