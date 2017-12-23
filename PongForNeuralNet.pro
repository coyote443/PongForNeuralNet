#-------------------------------------------------
#
# Project created by QtCreator 2017-12-19T18:03:38
#
#-------------------------------------------------

QT       += core gui

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
