#-------------------------------------------------
#
# Project created by QtCreator 2015-07-29T22:36:15
#
#-------------------------------------------------

QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = digital_clock
TEMPLATE = app
CONFIG += release

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

QMAKE_CXXFLAGS += -std=c++11
LIBS += -lGL -lX11 -lXext
