#-------------------------------------------------
#
# Project created by QtCreator 2015-04-14T18:41:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PearSimulation
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    simulationview.cpp \
    camera.cpp \
    sims/isimulation.cpp \
    sims/blocksimulation.cpp \
    sims/springsimulation.cpp \
    sims/watersimulation.cpp \
    interactors/iinteractor.cpp \
    interactors/camerainteractor.cpp \
    interactors/actorinteractor.cpp \
    actors/iactor.cpp \
    actors/rectangular.cpp \
    actors/gridbox.cpp \
    actors/emptyactor.cpp \
    actors/sphere.cpp \
    renderer/material.cpp \
    renderer/renderer.cpp \
    renderer/light.cpp \
    renderer/shader.cpp \
    renderer/environment.cpp \
    properties/ipropertytable.cpp \
    actors/mesh.cpp \
    loader/wavefrontloader.cpp \
    sims/poisson3d.cpp \
    actors/grid.cpp

HEADERS  += mainwindow.h \
    simulationview.h \
    camera.h \
    sims/isimulation.h \
    sims/blocksimulation.h \
    sims/springsimulation.h \
    sims/watersimulation.h \
    interactors/iinteractor.h \
    interactors/camerainteractor.h \
    interactors/actorinteractor.h \
    actors/iactor.h \
    actors/rectangular.h \
    actors/gridbox.h \
    actors/emptyactor.h \
    actors/sphere.h \
    datacontainer.h \
    datagrid.h \
    renderer/material.h \
    renderer/renderer.h \
    renderer/light.h \
    renderer/shader.h \
    renderer/environment.h \
    properties/ipropertytable.h \
    actors/mesh.h \
    loader/meshloader.h \
    loader/wavefrontloader.h \
    sims/poisson3d.h \
    actors/grid.h \
    utils/gradient.h

FORMS    += mainwindow.ui
