#ifndef ADAPTER_H
#define ADAPTER_H

/*
    This is a container for all available im clients.
    When creating IMService object, it will load available accounts,
    create clients for each account, and bind them together.
    This is a singletone class, all programs should be using the same instance.
   */

class IMService: public QObject
{
    class IMAccount
    {
        QString accountName, userName, password, server, port, memo;
    };

    Q_OBJECT
    public:
        IMService();//constructor
        void sendMsg(QString target, QString message, IMClient* client=0);//send a message to "target"
        QString presence(QString uri);//query whether a user is online
        QStringList friends(IMClient* client=0);//list all friends
        QList<IMClient*>& clients();//list all available clients
        ~IMService();//destructor
    public signals:
        void gotMsg(QString from, QString message, IMClient* client);//received a message from "from"
    private:
        QList<IMClient*> d_clients;//all available IM clients
        QList<IMAccount*> d_accounts;//account list
        void loadAccounts();//load all accounts from setting file
};

/* This is a virtual class that represents a IM channel, eg. a MSN client, an IRC client, or a XMPP client.
   It provides several virtual functions that needs to be implemented in the adaptors.
   */

class IMClient: public QObject
{
    Q_OBJECT
    public:
        IMClient();//constructor
        virtual void sendMsg(QString target, QString message);//send a message to "target"
        virtual void getPresence(QStringList& email, QStringList& status);//retrieve online friends and their status
        virtual ~IMClient()//destructor
    public signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
    private:
};

/* Adpator classes for various supported client types. All virtual functions must be implemented.
   */

class MSNClient: public IMClient
{
    Q_OBJECT
    public:
        MSNClient();//constructor
        void sendMsg(QString target, QString message);//send a message to "target"
        void getPresence(QStringList& email, QStringList& status);//retrieve online friends and their status
        ~MSNClient()//destructor
    public signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
    private:
};

class IRCClient: public IMClient
{
    Q_OBJECT
    public:
        IRCClient();//constructor
        void sendMsg(QString target, QString message);//send a message to "target"
        void getPresence(QStringList& email, QStringList& status);//retrieve online friends and their status
        ~IRCClient()//destructor
    public signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
    private:
};

class XMPPClient: public IMClient
{
    Q_OBJECT
    public:
        XMPPClient();//constructor
        void sendMsg(QString target, QString message);//send a message to "target"
        void getPresence(QStringList& email, QStringList& status);//retrieve online friends and their status
        ~XMPPClient()//destructor
    public signals:
        void connected();//client connected
        void disconnected();//client disconnected
        void gotMsg(QString from, QString message);//client got a message
        void error(QString errorMsg);//client encountered an error
    private:
};


#endif
