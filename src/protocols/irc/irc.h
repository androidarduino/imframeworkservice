#ifndef IRC_H
#define IRC_H
#include <QThread>
#include <QTcpSocket>
#include <QStringList>

class IRCChannel
{
    public:
        IRCChannel(){}
        QString d_name, d_description;
        int d_headCount;
        QStringList d_users;
};

class IRCClient: public QObject
{
    Q_OBJECT
    public:
        IRCClient(QString server, int port);
        void connect(QString& server, int port);
        void connect();
        QStringList channels();
        QStringList users(QString channel="");
        void join(QString channel);
        void send(QString channelOrUser, QString msg);
        void away(QString autoReplyMessage);
        void disconnect();
        bool isConnected();
        void login();
    signals:
        void message(QString from, QString fromURI, QString receiver, QString msg);
        void notification(QString msg);
        void invitation(QString from, QString toChannel);
        void connected();
        void ping(QString& ping);
        void join(QString username, QString channel, QString uri);
        void disconnected();
        void updated();
    private slots:
        void msgArrived();
    private:
        void gotPing(QString& msg);
        void gotMsg(QString& msg);
        void gotChannelList(QString& msg);
        void gotUserNames(QString& msg);
        void gotNotification(QString& msg);
        void gotInvitation(QString& msg);
        void gotMessageOfTheDay(QString& msg);
        void gotJoin(QString& msg);
        void sendCommand(QString cmd);
        void pendCommand(QString cmd);
    private:
        QTcpSocket d_socket;
        int d_port;
        QStringList d_commands;
    public:
        QString d_server;
        bool d_connected;
        bool d_autoReconnect;
        QString d_userName, d_realName, d_password;
        int d_userMode;
        QString d_status;
        QList<IRCChannel*> d_channels;
        QStringList d_motd;
};

/*
class IRCPlugin: public QObject, public IMProtocol
{
    Q_OBJECT
    Q_INTERFACES(IMProtocol)
    public:
<<<<<<< .mine
        IRCProtocol(Msg& parameters);
        ~IRCProtocol();
=======
        IRCPlugin();
        ~IRCPlugin();
	bool available();
	QString& operator[](QString ref);
	QString test(){return QString("test message return");}
>>>>>>> .r68
		bool available();
		QList<Msg> onlineBuddies();
		QString& operator[](QString propertyName);
    signals:
<<<<<<< .mine
        void msgArrived(Msg& msg);
		void statusChanged(QString status);
=======
        void msgArrived(Msg& msg);
        void statusChanged(QString status);
>>>>>>> .r68
    public slots:
<<<<<<< .mine
        void sendMsg(Msg& msg);
=======
        void sendMsg(Msg&msg);
>>>>>>> .r68
        void login();
	QList<Msg> onlineBuddies();
    private:
        IRCClient* d_client;
    private slots:
        void gotMsg(QString from, QString fromUri, QString receiver, QString msg);
};
*/
#endif
