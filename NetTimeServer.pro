#-------------------------------------------------
#
# Project created by QtCreator 2017-11-07T08:12:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetTimeServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timeservice.cpp \
    configinfo.cpp \
    timegenerater.cpp \
    netsender.cpp \
    timeadjuster.cpp \
    log.cpp

HEADERS  += mainwindow.h \
    timeservice.h \
    configinfo.h \
    timegenerater.h \
    netsender.h \
    common.h \
    timeadjuster.h \
    log.h

FORMS    += mainwindow.ui
