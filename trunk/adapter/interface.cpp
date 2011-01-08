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
    emit get(from, msg, replyTo);
}

void Messenger::subscribe(QString receiver)
{
    d_subscribers<<receiver;
}

long long Messenger::send(QVariant msg, QString receiver)
{
    qDebug()<<"messenger sending :"<<receiver<<msg;
    if(receiver=="")
    {
        return d_service->sendMsg(d_subscribers, d_resourceType, msg.toString());
    }
    return d_service->sendMsg(receiver, d_resourceType, msg.toString());
}

long long Messenger::send(QVariant msg, QStringList receiver)
{
    qDebug()<<"messenger sending :"<<receiver<<msg;
    if(!receiver.empty())
        return d_service->sendMsg(receiver, d_resourceType, msg.toString());
    return d_service->sendMsg(d_subscribers, d_resourceType, msg.toString());
}

Presence::Presence()
{
    connect(d_service, SIGNAL(updated()), this, SIGNAL(updated()));
}

QString Presence::getPresence(QString buddy)
{
    return d_service->presence(buddy);
}

void monitor(QString buddy, bool subscribe)
{
    Q_UNUSED(buddy);
    Q_UNUSED(subscribe);
    //TODO: wait for implementation in service and clients
}

QStringList Presence::allOnline()
{
    return d_service->friends();
}

}