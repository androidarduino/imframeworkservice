#ifndef SERVICE_H
#define SERVICE_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QStringList>
#include <QMap>
#include "improtocol.h"

class Msg
{
 /*
    service to client message protocol:
    "<msg protocol=irc1 app=messagingclient threadId=1>this is a message</msg>"
    "<msg protocol=IMServer app=registerService name=messagingclient status=online></msg>

   */
    public:
        Msg(QByteArray msg);
        QString& operator [](QString& name);
    private:
        QMap<QString, QString> items;

};

class IMProtocol:public QObject
{
	Q_OBJECT
	public:
		IMProtocol();
		~IMProtocol();
	public signals:
		void msgArrived(Msg& msg);
	public slots:
		void sendMsg(Msg& msg);
		void login();
};

class IMClient:public QLocalSocket
{
    Q_OBJECT
    public:
        IMClient(){}
        ~IMClient(){}
        QString name;
};

class IMServerManager:public QObject
{
    Q_OBJECT
    public:
        IMServerManager();
        ~IMServerManager();
    protected:
        void processClientRequest(IMClient* client, Msg& msg);
        void registerClient(QString appName, IMClient* client);

class IMService: public IMServerManager
{
    Q_OBJECT
    public:
        IMService();
        ~IMService();
        bool startProtocol(QString& name);
        bool stopProtocol(QString& name);
    private:
        QLocalServer d_server;
        QList<IMClient*> clients;
        QList<IMProtocol*> protocols;
    private slots:
        void newConnection();
        void clientMsg();
        void protocolMsg();
};

#endif

