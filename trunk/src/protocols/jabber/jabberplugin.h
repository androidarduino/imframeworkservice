#ifndef Jabber_PLUGIN_H
#define Jabber_PLUGIN_H

#include "../../service/protocolinterface.h"
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QXmppClient.h>
#include <QXmppMessage.h>

class IMJabberPlugin: public QObject, public IMProtocol
{
    Q_OBJECT
    Q_INTERFACES(IMProtocol);
    public:
        IMJabberPlugin();
		void init(QString);
        ~IMJabberPlugin();
        bool available();
        QList<Msg> onlineBuddies();
        QString& operator[](QString);
        void sendMsg(Msg&msg);
        void login();
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
