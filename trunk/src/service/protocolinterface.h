#ifndef IMPROTOCOLINTERFACE_H
#define IMPROTOCOLINTERFACE_H

#include <QString>
#include <QList>
#include "../msg.h"

/*
    Abstract of the protocols, used to load/unload the protocol instances, pass it to the server, and send messages for server.
   */

class IMProtocol:public QObject
{
	Q_OBJECT
	public:
		virtual IMProtocol(Msg& parameters);
		virtual ~IMProtocol();
        virtual bool available();
		virtual QList<Msg> onlineBuddies();
		virtual QString& operator [](QString propertyName);
	signals:
		virtual void msgArrived(Msg& msg);
		virtual void statusChanged(QString status);
	public slots:
		virtual void sendMsg(Msg& msg);
		virtual void login();
    private:
        Msg d_properties;
};

Q_DECLARE_INTERFACE(IMProtocol, "com.vrcats.IMFramework.IMProtocol");

#endif
