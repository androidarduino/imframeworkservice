######################################################################
# Automatically generated by qmake (2.01a) Fri Sep 9 17:28:32 2011
######################################################################

CONFIG += qtestlib
CONFIG -= qtgui
QT += network
TEMPLATE = app
TARGET = testclient
DEPENDPATH += .
INCLUDEPATH += . ..

# Input
HEADERS += ../server.h ../../msg.h
SOURCES += servertests.cpp ../server.cpp ../../msg.cpp
