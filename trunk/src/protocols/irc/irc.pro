######################################################################
# Automatically generated by qmake (2.01a) ?? 11? 16 14:48:24 2010
######################################################################
TEMPLATE = lib
CONFIG += plugin
TARGET = irc
DESTDIR = ../../service/protocolplugins/
DEPENDPATH += .
INCLUDEPATH += .
QT += network

# Input
HEADERS += irc.h ../../service/protocolinterface.h ../../msg.h
SOURCES += irc.cpp ircplugin.cpp ../../msg.cpp
