#ifndef Jabber_PLUGIN_H
#define Jabber_PLUGIN_H

#include "protocolinterface.h"
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QXmppClient.h>
#include <QXmppMessage.h>
#include <QXmppLogger.h>

class IMJabberPlugin: public QObject, public IMProtocol
{
    Q_OBJECT
    Q_INTERFACES(IMProtocol);
    public:
        IMJabberPlugin();
		void init(Msg& config);
        ~IMJabberPlugin();
        bool available();
        Msg& onlineBuddies();
        QString& operator[](QString);
        void sendMsg(Msg&msg);
        void login();
		bool canDo(QString&);
		bool canSendTo(QString&);
    signals:
        void msgArrived(Msg& msg);
        void statusChanged(QString status);
	private:
		QXmppClient* d_client;
		QString d_user, d_pass, d_host, d_port, d_domain;
		void messageReceived(const QXmppMessage&);
    private slots:
        void gotMsg(QString from, QString fromUri, QString receiver, QString msg);
};

#endif
