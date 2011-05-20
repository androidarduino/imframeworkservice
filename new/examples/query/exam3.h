#ifndef EXAM3_H
#define EXAM3_H

//#include "../../interface.h"
#include <QApplication>
#include <QtGui>
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <iostream>
#include <string>
/*
    This example is to demonstrate how to use the Query class.
    In this example, we implemented a sql query machine. User can send a query to a remote machine and get the result asap.
   */

//this class sends the question
class QuerySender: public QObject
{
    Q_OBJECT
    public:
        QuerySender();
        void linkTo(Group server);
        void query(QString sqlQuery, QString format="");
    public signals:
        void getAnswer(Group from, QVariant results);
};

//this class answers the question with preferred format
class QueryServer: public QObject
{
    Q_OBJECT
    public:
        QueryServer();
    public slots:
        void gotQuery(QString sqlQuery, QString format);
    private:
        void answer(Group receiver, QVariant results);
};

#endif
