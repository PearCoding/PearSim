CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PS-Plugin-Spring)

SET(_HDR ${PSP_Root}/spring/springsimulation.h)
QT5_WRAP_CPP(_HDR_Moc ${_HDR})

add_library(ps_spring_plugin MODULE ${PSP_Root}/spring/springsimulation.cpp ${_HDR} ${_HDR_Moc})
target_link_libraries(ps_spring_plugin PearSimLib)
target_link_libraries(ps_spring_plugin Qt5::Widgets)
set_target_properties(ps_spring_plugin PROPERTIES VERSION ${PS_Version})
set_target_properties(ps_spring_plugin PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
