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
    isimulation.cpp \
    sims/blocksimulation.cpp \
    camera.cpp \
    utils/gridbox.cpp \
    utils/actor.cpp

HEADERS  += mainwindow.h \
    simulationview.h \
    isimulation.h \
    sims/blocksimulation.h \
    camera.h \
    utils/gridbox.h \
    utils/actor.h

FORMS    += mainwindow.ui
