#ifndef SERVICE_H
#define SERVICE_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QStringList>
#include <QMap>
#include <QByteArray>
#include <QRegExp>
#include <QDebug>
#include "protocolinterface.h"

/*
    Abstract of messages transfering between server and client/protocol, convert a message string to data structure, and back
   */

class Msg
{
 /*
    service to client message protocol:
    "<msg protocol=irc1 app=messagingclient threadId=1>this is a message</msg>"
    "<msg protocol=IMServer app=registerService name=messagingclient status=online></msg>
   */
    public:
        Msg(QByteArray msg);
        QString operator [](QString name);
	void print();
        QString d_string;
    private:
        QMap<QString, QString> items;
};

/*
   Abstract of the client delegation, talk to the client program on behalf of the server.
   */

class IMClient:public QObject
{
    Q_OBJECT
    public:
        IMClient(QLocalSocket* socket);
        ~IMClient(){}
        QString name;
	QLocalSocket* d_socket;
};

/*
   Base class of the server, process some server sepecific matters, like register/unregister clients, etc.
    */

class IMServerManager:public QObject
{
    Q_OBJECT
    public:
        IMServerManager();
        ~IMServerManager();
    protected:
        void processClientRequest(IMClient* client, Msg& msg);
        void registerClient(QString appName, IMClient* client);
};

/*
   Main part of the server
   */

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
