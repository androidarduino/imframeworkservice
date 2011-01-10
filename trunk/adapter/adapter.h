#ifndef ADAPTER_H
#define ADAPTER_H

#include <QStringList>
#include <QSettings>
#include "improtocols.h"
using namespace irc;

/* at this stage we skip the communications between service and interfaces to simplify the project.
  when all the functions work properly, we sill consider to add a soap interface between them,
  so that other application developers and non-c++ developers can make use of our service
  */

class IMClient;

class IMAccount//helper class to store one IM account
{
    public:
        QString accountName, type, userName, password, server, port, groups, memo;
};

/*
    This is a container for all available im clients.
    When creating IMService object, it will load available accounts,
    create clients for each account, and bind them together.
    This is a singletone class, all programs should be using the same instance.
*/
class IMService: public QObject
{
    Q_OBJECT
    public:
        IMService();//constructor
    //self raising members:
        void start();//start and login all clients
        void stop();//stop, logout all clients
    //communication members:
        long long sendMsg(QString target, QString app, QString message, IMClient* client=0);//send a message to "target"
        long long sendMsg(QStringList targets, QString app,  QString message, IMClient* client=0);//send a message to "target"
        QString presence(QString uri);//query whether a user is online
        QStringList friends(IMClient* client=0);//list all friends
        ~IMService();//destructor
    signals:
        void gotMsg(QString from, QString dest, QString message, long long answerTo);//received a message from "from"
        void updated();
    private slots:
        void receivedMsg(QString from, QString message);//function to process received message
    //resource management members:
    private:
        int d_listenPort;//socket port to listen on
        void registerResource(QString resourceName);//app register a resource
        void unregisterResource(QString resourceName);//app unregister a resource
        bool sendMessageToResource(QString resourceName, QString from, QVariant message);//send a message to a registered resource
        void receivedMessageFromResource(QString resourceName, QString to, QVariant message);//got a message from a registered resource, forward it to the destination
    //account and client management members
    private:
        QList<IMClient*> d_clients;//all available IM clients
        QList<IMAccount> d_accounts;//account list
        long long d_id;
    public:
        QList<IMClient*>& clients();//list all available clients
};

/* This is a virtual class that represents a IM channel, eg. a MSN client, an IRC client, or a XMPP client.
   It provides several virtual functions that needs to be implemented in the adaptors.
   */

class IMClient: public QObject
{
    Q_OBJECT
    public:
        IMClient(IMAccount& account);//constructor
        bool canDo(QString serviceType);//query whether the client has the ability to do something
        virtual void login()=0;//login to account
        virtual void logout()=0;//logout from account
        virtual void sendMsg(QString target, QString message)=0;//send a message to "target"
        virtual QStringList getPresence()=0;//retrieve online friends and their status
        QString name(){return d_accountName;}
        virtual bool hasUser(QString user);
        virtual ~IMClient();//destructor
    signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
        void updated();
    protected:
        QString d_accountName, d_userName, d_password, d_server, d_port, d_groups, d_memo;
    private slots:
        void offline();
    public:
        QStringList onlineBuddies;
        bool available;
    public slots:
        void update();
};

/* Adpator classes for various supported client types. All virtual functions must be implemented.
   */
/*
class MSNIMClient: public IMClient
{
    Q_OBJECT
    public:
        MSNIMClient(IMAccount& account);//constructor
        void sendMsg(QString target, QString message);//send a message to "target"
        QStringList getPresence();//retrieve online friends and their status
        ~MSNIMClient();//destructor
    signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
    private:
};*/

class IRCIMClient: public IMClient
{
    Q_OBJECT
    public:
        IRCIMClient(IMAccount& account);//constructor
        void sendMsg(QString target, QString message);//send a message to "target"
        QStringList getPresence();//retrieve online friends and their status
        ~IRCIMClient();//destructor
        void login();//login
        void logout();//logout
        bool hasUser(QString user);//check whether he is in this client
    signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
    private:
        irc::IRCClient* client;
    private slots:
        void gotIRCMessage(QString from, QString fromURI, QString receiver, QString msg);
        void update();
};

class XMPPIMClient: public IMClient
{
    Q_OBJECT
    public:
        XMPPIMClient(IMAccount& account);//constructor
        void sendMsg(QString target, QString message);//send a message to "target"
        QStringList getPresence();//retrieve online friends and their status
        ~XMPPIMClient();//destructor
        void login();//login
        void logout();//logout
    signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
    private:
        QXmppClient* client;
    private slots:
        void gotXmppMessage(const QXmppMessage& message);
};

class IMServiceTester: public QObject
{
    Q_OBJECT
    public:
        IMServiceTester()
        {
            service.start();
        }
    public slots:
        void gotMsg(QString from, QString message, long answerTo, IMClient* client)
        {
            Q_UNUSED(answerTo);
            Q_UNUSED(client);
            service.sendMsg(from, "", message);
        }
    private:
    IMService service;
};



#endif
