#ifndef IMPROTOCOLINTERFACE_H
#define IMPROTOCOLINTERFACE_H

#include <QString>
#include <QList>
#include "../msg.h"
#include <QtPlugin>

/*
   Abstract of the protocols, used to load/unload the protocol plugin instances, pass it to the server, and send messages for server.
   */

class IMProtocol
{
    public:
        IMProtocol(){}
        virtual ~IMProtocol(){}
        virtual bool available()=0;
        virtual QList<Msg> onlineBuddies()=0;
        virtual QString& operator [](QString propertyName)=0;
    signals:
        virtual void msgArrived(Msg& msg)=0;
        virtual void statusChanged(QString status)=0;
    public:
        virtual void sendMsg(Msg& msg)=0;
        virtual void login()=0;
        virtual QString test()=0;
    private:
        Msg d_properties;
};

Q_DECLARE_INTERFACE(IMProtocol, "com.vrcats.IMFramework.IMProtocol/1.0");

#endif
