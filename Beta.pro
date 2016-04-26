QT += core
QT -= gui

CONFIG += c++11

TARGET = Beta
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    leptonSPI.cpp \
    TCP_Server.cpp

HEADERS += \
    leptonSPI.h \
    TCP_Server.h
