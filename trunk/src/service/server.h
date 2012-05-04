#ifndef SERVICE_H
#define SERVICE_H
#include <QLocalServer>
#include <QLocalSocket>
#include <QStringList>
#include <QMap>
#include <QByteArray>
#include <QRegExp>
#include <QDebug>
#include <QtPlugin>
#include <QPluginLoader>
#include "protocolinterface.h"
#include "../msg.h"

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
class IMServerManager:public QObject, public IMProtocol
{
    Q_OBJECT
    Q_INTERFACES(IMProtocol)
    public:
        IMServerManager();
        ~IMServerManager();
    protected:
        void processClientRequest(IMClient* client, Msg& msg);
        void registerClient(QString appName, IMClient* client);
    public:
        bool available();
        QList<Msg> onlineBuddies();
        QString& operator [](QString propertyName);
    public slots:
        void sendMsg(Msg& msg);
        void login();
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
        bool controlProtocol(QString& name, QString& command);
    private:
        QLocalServer d_server;
        QList<IMClient*> d_clients;
        QPluginLoader d_loader;
        void loadPlugins();
    private slots:
        void newConnection();
        void clientMsg();
        void protocolMsg(Msg& msg);
};


#endif
