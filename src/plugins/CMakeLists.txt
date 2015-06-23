CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

SET(PSP_Root ${PS_Root}/plugins)

add_subdirectory(${PSP_Root}/block)
add_subdirectory(${PSP_Root}/poisson3d)
add_subdirectory(${PSP_Root}/spring)
add_subdirectory(${PSP_Root}/water)