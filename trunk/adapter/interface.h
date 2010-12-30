#ifndef INTERFACE_H
#define INTERFACE_H

/*
    In this file a set of interface classes were defined.
    These interface classes provide developer the ability of communicating to other parties easily and freely.
*/

namespace IMFramework
{

class Presence:public QObject
/*
    retrieve the presence of buddies
    query the presence and status of one buddy
    subscribe to the presence change of a group of people
*/
{
    QOBJECT
    public:
        Presence();

};

class Messenger:public QObject
/*
    send a single message
    receive messages from others
    receive replies from others regarding sent message
*/
{
    QOBJECT
    public:
        Messenger();
        long long send(QVariant msg, Group receiver="");
        void linkTo(QString receiver);
    public signals:
        void received(QVariant msg, Group from, long long replyTo);
};

class SyncAble:public QObject
/*
    subscribe to a buddy or a group
    auto or manual update to synchronize
    broadcast changes to subscribers
*/
{
    Q_OBJECT
    public:
        SyncAble();
        void linkTo(Group syncWith);
        virtual QString toString();
        virtual void fromString();
        void pull(Group fromSrc="");
        void push(Group toDest="");
    public signal:
        void updated();
};

class Query:public QObject
/*
    send a query with preferable reply format
    receive replies
*/
{
    QOBJECT
    public:
        Query();
        void linkTo(Group server);
        long long query(QVariant query, QString preferableFormat);
    public signals:
        void results(QVariant result, long long answerTo);
};
}

#endif
