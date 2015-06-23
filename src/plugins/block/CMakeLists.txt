CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PS-Plugin-Block)

SET(_HDR ${PSP_Root}/block/blocksimulation.h)
QT5_WRAP_CPP(_HDR_Moc ${_HDR})

add_library(ps_block_plugin MODULE ${PSP_Root}/block/blocksimulation.cpp ${_HDR} ${_HDR_Moc})
target_link_libraries(ps_block_plugin PearSimLib)
target_link_libraries(ps_block_plugin Qt5::Widgets)
set_target_properties(ps_block_plugin PROPERTIES VERSION ${PS_Version})
set_target_properties(ps_block_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
