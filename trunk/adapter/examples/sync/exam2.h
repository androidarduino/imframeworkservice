#ifndef EXAM2_H
#define EXAM2_H

//#include "../../interface.h"
#include <QApplication>
#include <QtGui>
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <iostream>
#include <string>
/*
    This example is to demonstrate how to use the Synchronizable class.
    In this example, we implemented simple text box, and synchronize it to a remove text box
   */
class SyncTextBox
: public SyncAble
{
    Q_OBJECT
    public:
        SyncTextBox();
        QString toString();
        void fromString();
        void syncTo(Group grp);
    public slots:
        void textTyped();//connect this to the text typed signal and call push
        void textUpdated();//connect this to the text typed signal and update the text
    private:
        QTextEdit d_edit;
};
#endif
