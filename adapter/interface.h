#ifndef INTERFACE_H
#define INTERFACE_H

#include "adapter.h"
/*
    In this file a set of interface classes were defined.
    These interface classes provide developer the ability of communicating to other parties easily and freely.
*/

namespace IMFramework
{

class IMInterface: public QObject
{
    Q_OBJECT
    public:
        IMInterface(QString type="");//constructor
        void setResourceType(QString type);//constructor
        virtual ~IMInterface();//destructor
    protected:
        static IMService* d_service;
        QString d_resourceType;
};

class Presence:public IMInterface
/*
    retrieve the presence of buddies
    query the presence and status of one buddy
    subscribe to the presence change of a group of people
*/
{
    Q_OBJECT
    public:
        Presence();//constructor
        QString getPresence(QString buddy);//query buddy's status
        void monitor(QString buddy, bool subscribe=true);//monitor or unmonitor a buddy's presence
        QStringList allOnline();//get all online buddies
    signals:
        void presenceChanged(QString buddy, QString status);//emit when monitored buddy status change, or new buddy comes online
        void updated();//when there is a general change
};

class Messenger:public IMInterface
/*
    send a single message
    receive messages from others
    receive replies from others regarding sent message
*/
{
    Q_OBJECT
    public:
        Messenger(QString type="");//constructor
        void subscribe(QString receiver);//link to a group of people, by default the msg will be sent to the group of people
        long long send(QVariant msg, QString receiver="");//send msg to a single receiver
        long long send(QVariant msg, QStringList receiver);//send msg to a group of people
        void sendPlainMsg(QString msg, QString receiver="");//send a plain message without prefixes
    signals:
        void get(QVariant msg, QString from, long long replyTo);//emited when got a message from someone
    private slots:
        void gotMsg(QString from, QString dest, QString msg, long long replyTo);
    protected:
        QStringList d_subscribers;
};

class Synchronizor:public IMInterface
/*
    subscribe to a buddy or a group
    auto or manual update to synchronize
    broadcast updates to subscribers
*/
{
    Q_OBJECT
    public:
        Synchronizor(QStringList syncWith=QStringList());//constructor
        void pushFull(QVariant data, QStringList toDest=QStringList());//push full data to all subscribers
    signals:
        void updateFull(QVariant data);//notify there is an update
    public:
        QStringList subscribers;//exposing this variable will save 4 subscribing and unsubscribing apis
};

class Query:public IMInterface
/*
    send a query with preferable reply format
    receive replies
*/
{
    Q_OBJECT
    public:
        Query();
        void subscribe(QStringList servers);//query to a group of servers
        long long ask(QVariant query, QString preferableFormat);//send a query to all servers
    signals:
        void reply(QVariant result, long long answerTo, QString fromServer);//emit when got reply from servers
};
}

#endif
