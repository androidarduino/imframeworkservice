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
        IMInterface();//constructor
        virtual ~IMInterface();//destructor
    protected:
        static IMService* d_service;
};

IMInterface::d_service=0;

IMInterface::IMInterface()
{
    if(d_service==0)
    {
        d_service=new IMService();
        d_service->start();
    }
}

IMInterface::~IMInterface()
{

}

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
        Messenger();//constructor
        void subscribe(QString receiver);//link to a group of people, by default the msg will be sent to the group of people
        long long send(QVariant msg, QString receiver="");//send msg to a single receiver
        long long send(QVariant msg, QStringList receiver);//send msg to a group of people
    signals:
        void get(QString msg, QString from, long long replyTo);
    private slots:
        void gotMsg(QVariant msg, QString from, long long replyTo, IMClient* client);
    protected:
        QStringList d_subscribers;
};

Messenger::Messenger()
{
    connect(d_service, SIGNAL(gotMsg(QString, QString, long long, IMClient*)), this, SLOT(gotMsg(QString, QString, long
    long,
    IMClient*)));
}

Messenger::gotMsg(QString from, QString msg, long long replyTo, IMClient* client)
{
    emit get(from, msg, replyTo);
}

void Messenger::subscribe(QString receiver)
{
    subscribers<<receiver;
}

long long send(QVariant msg, QString receiver="")
{
    if(receiver=="")
    {
        return d_service.sendMsg(d_subscribers, msg.toString());
    }
    return d_service.sendMsg(receiver, msg.toString());
}

long long send(QVariant msg, QStringList receiver)
{
    return d_service.sendMsg(d_subscribers, msg.toString());
}




class Synchronizor:public IMInterface
/*
    subscribe to a buddy or a group
    auto or manual update to synchronize
    broadcast changes to subscribers
*/
{
    Q_OBJECT
    public:
        Synchronizor();//constructor
        void subscribe(QStringList syncWith);//subscribe to a group of people
        void pull(QStringList fromSrc=QStringList());//get update from subscriptions
        void push(QStringList toDest=QStringList());//push chagne to subscriptions
    signals:
        void updated();//notify there is a change
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