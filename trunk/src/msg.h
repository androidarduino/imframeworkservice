#ifndef MSG_H
#define MSG_H

#include <QString>
#include <QByteArray>
#include <QRegExp>
#include <QDebug>
#include <QMap>

/*
    Abstract of messages transfering between server and client/protocol, convert a message string to data structure, and back
   */

class Msg
{
 /*
    service to client message protocol:
    "<msg protocol=irc1 app=messagingclient threadId=1>this is a message</msg>"
    "<msg protocol=IMServer app=registerService name=messagingclient status=online></msg>
   */
    public:
        Msg(){}
        Msg(QByteArray msg);
        QString operator [](QString name);
	void print();
        QString d_string;
    private:
        QMap<QString, QString> items;
};

#endif
