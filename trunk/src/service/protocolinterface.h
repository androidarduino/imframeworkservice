#ifndef IMPROTOCOLINTERFACE_H
#define IMPROTOCOLINTERFACE_H

#include <QString>

/*
    Abstract of the protocols, used to load/unload the protocol instances, pass it to the server, and send messages for server.
   */

class IMProtocol:public QObject
{
	Q_OBJECT
	public:
		virtual IMProtocol(Msg& parameters);
		virtual ~IMProtocol();
		virtual QString& name();
                virtual bool available();
	signals:
		virtual void msgArrived(Msg& msg);
	public slots:
		virtual void sendMsg(Msg& msg);
		virtual void login();
        private:
                bool d_available;
                QString d_name;
};

Q_DECLARE_INTERFACE(IMProtocol, "com.vrcats.IMFramework.IMProtocol");

#endif
