######################################################################
# Automatically generated by qmake (2.01a) Thu May 21 16:58:03 2009
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += network
QT += xml

UI_DIR=./tmpdata
MOC_DIR=./tmpdata
OBJECTS_DIR=./tmpdata
RCC_DIR=./tmpdata

# Input
HEADERS += md5.h \
           mime.h \
           msn.h \
           msnauth.h \
           msncontact.h \
           contactlist.h \
           msnsession.h \
           switchboard.h
FORMS += settings.ui
SOURCES += main.cpp \
           md5.cpp \
           msn.cpp \
           msnauth.cpp \
           msnsession.cpp \
           contactlist.cpp \
           switchboard.cpp
RESOURCES += msn.qrc