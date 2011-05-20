#include "interface.h"
namespace IMFramework
{

IMService* IMInterface::d_service=0;

IMInterface::IMInterface(QString type)
{
    d_resourceType=type;
    if(d_service==0)
    {
        d_service=new IMService();
        d_service->start();
    }
    connect(d_service, SIGNAL(gotMsg(QString, QString, QString, long long)), this, SLOT(gotMsg(QString, QString, QString, long long)));
}

void IMInterface::setResourceType(QString type)
{
    d_resourceType=type;
}

IMInterface::~IMInterface()
{
    //no necessary to delete the service
}

Messenger::Messenger(QString type):IMInterface(type)
{
    connect(d_service, SIGNAL(gotMsg(QString, QString, QString, long long)), this, SLOT(gotMsg(QString, QString,
    QString, long long)));
}

void Messenger::gotMsg(QString from, QString dest, QString msg, long long replyTo)
{
    qDebug()<<"messenger got message"<<from<<dest<<msg<<replyTo;
    Q_UNUSED(dest);
    //TODO: check whether it is for this app
    //TODO: here convert qstring to qvariant
    emit get(msg, from, replyTo);
}

void Messenger::subscribe(QString receiver)
{
    d_subscribers<<receiver;
}

long long Messenger::send(QVariant msg, QString receiver)
{
    QString m=QString(msg.typeName())+":"+msg.toString();
    qDebug()<<"messenger sending :"<<receiver<<m;
    if(receiver=="")
    {
        return d_service->sendMsg(d_subscribers, d_resourceType, m);
    }
    return d_service->sendMsg(receiver, d_resourceType, m);
}

long long Messenger::send(QVariant msg, QStringList receiver)
{
    qDebug()<<"messenger sending :"<<receiver<<msg;
    if(!receiver.empty())
        return d_service->sendMsg(receiver, d_resourceType, msg.toString());
    return d_service->sendMsg(d_subscribers, d_resourceType, msg.toString());
}

void Messenger::sendPlainMsg(QString msg, QString receiver)
{
    Q_UNUSED(msg);
    Q_UNUSED(receiver);
    //TODO: wait for the service to implement the according functions
}

Presence::Presence()
{
    connect(d_service, SIGNAL(updated()), this, SIGNAL(updated()));
}

QString Presence::getPresence(QString buddy)
{
    return d_service->presence(buddy);
}

void Presence::monitor(QString buddy, bool subscribe)
{
    Q_UNUSED(buddy);
    Q_UNUSED(subscribe);
    //TODO: wait for implementation in service and clients
}

QStringList Presence::allOnline()
{
    return d_service->friends();
}

Synchronizor::Synchronizor(QStringList syncWith)
{
    if(syncWith.empty())
        return;
    subscribe(syncWith);
}

void Synchronizor::gotMsg(QString from, QString dest, QString msg, long long replyTo)
{
    //TODO: check whether it is for this application
    //TODO: extract the synchronization information, and emit updateFull() signal
}

void pushFull(QVariant data, QStringList toDest)
{
    //push current data to the destinations
}

Query::Query(QString server)
{
    //construct a query
}

long long ask(QVariant query, QString preferableFormat)
{
    //issue a query to the server
}

void Query::gotMsg(QString from, QString dest, QString msg, long long replyTo)
{
    //process incoming messages
    //TODO: check whether it is for this application
    //TODO: extract query results
}

}
