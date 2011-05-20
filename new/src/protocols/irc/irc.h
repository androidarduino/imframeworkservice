#ifndef IRC_H
#define IRC_H
#include <QThread>
#include <QTcpSocket>
#include <QStringList>

namespace irc
{

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
        void login();
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
        QString d_server;
        int d_port;
        QStringList d_commands;
        bool d_connected;
    public:
        bool d_autoReconnect;
        QString d_userName, d_realName, d_password;
        int d_userMode;
        QString d_status;
        QList<IRCChannel*> d_channels;
        QStringList d_motd;
};

}
#endif
