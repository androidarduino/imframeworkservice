#ifndef EXAM1_H
#define EXAM1_H

#include "interface.h"
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
class VRIM: public QLineEdit
{
    Q_OBJECT
    public:
        VRIM();
    protected:
        void keyPressEvent(QKeyEvent* event);
    private slots:
        void get(QString msg, QString from, long long replyTo);
    private:
        IMFramework::Messenger* messenger;
        QLineEdit dest;
        QTextEdit incoming;
};
#endif
