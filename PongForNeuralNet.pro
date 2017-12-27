#-------------------------------------------------
#
# Project created by QtCreator 2017-12-19T18:03:38
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PongForNeuralNet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        pongwidget.cpp \
        pongmodel.cpp

HEADERS  += mainwindow.h \
    pongwidget.h \
    pongmodel.h

FORMS    += mainwindow.ui

DISTFILES += \
    data/Gate.wav \
    data/Pallet.wav \
    data/Wall.wav \
    data/ACDC.wav

DISTFILES += \
    data/AddNoise.ico \
    data/SoundOff.ico \
    data/SoundOn.ico \
    data/StartGame.ico

RESOURCES += \
    resources.qrc
