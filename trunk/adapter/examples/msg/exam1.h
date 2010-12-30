#ifndef EXAM1_H
#define EXAM1_H

//#include "../../interface.h"
#include <QApplication>
#include <QtGui>
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <iostream>
#include <string>
/*
    This example is to demonstrate how to use the Messenger class.
    In this example, we implemented a cross protocol messenger, with which you can send/receive message.
    The interface part is skipped.
   */
class VRIM
: public QObject
{
    Q_OBJECT
    public:
        VRIM();
        void run();
};
#endif
