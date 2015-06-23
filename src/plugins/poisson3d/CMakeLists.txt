CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PS-Plugin-Poisson3D)

SET(_HDR ${PSP_Root}/poisson3d/poisson3d.h)
QT5_WRAP_CPP(_HDR_Moc ${_HDR})

add_library(ps_poisson3d_plugin MODULE ${PSP_Root}/poisson3d/poisson3d.cpp ${_HDR} ${_HDR_Moc})
target_link_libraries(ps_poisson3d_plugin PearSimLib)
target_link_libraries(ps_poisson3d_plugin Qt5::Widgets)
set_target_properties(ps_poisson3d_plugin PROPERTIES VERSION ${PS_Version})
set_target_properties(ps_poisson3d_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
