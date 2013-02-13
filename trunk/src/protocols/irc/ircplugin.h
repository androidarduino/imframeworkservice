#ifndef IRC_PLUGIN_H
#define IRC_PLUGIN_H

#include "protocolinterface.h"
#include "irc.h"
#include <QDebug>
#include <QString>
#include <QByteArray>

class IMIRCPlugin: public QObject, public IMProtocol
{
    Q_OBJECT
    Q_INTERFACES(IMProtocol);
    public:
        IMIRCPlugin();
        void init(Msg& config);
        ~IMIRCPlugin();
        bool available();
        QObject* getObject();
        Msg& onlineBuddies();
        QString& operator[](QString);
        void sendMsg(Msg& msg);
        void login();
		bool canDo(QString&);
		bool canSendTo(QString&);
    signals:
        void msgArrived(Msg& msg);
        void statusChanged(QString status);
	private:
		IRCClient* d_client;
		QString d_host, d_port, d_user, d_pass;
    private slots:
        void gotMsg(QString from, QString fromUri, QString receiver, QString msg);
};

#endif
