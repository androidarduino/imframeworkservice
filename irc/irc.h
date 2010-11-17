#ifndef IRC_H
#define IRC_H
#include <QThread>
#include <QTcpSocket>
#include <QStringList>

/*class IRCReplies
{

welcome message:
    ":(.+) 001 (.+) :(.*)"
:irc.troll.no 001 vrcats :Welcome to the Internet Relay Network vrcats!~1@172.30.136.110

motd reply:
    ":(.+) 372 (.+) :(.*)"
//:irc.troll.no 375 vrcats :- irc.troll.no Message of the Day -
//:irc.troll.no 372 vrcats :- 5/6/2007 18:18
//:irc.troll.no 376 vrcats :End of MOTD command.

notification message:
    ":(.+) NOTICE (.+) :(.*)"
//:irc.troll.no NOTICE vrcats :Server is currently in split-mode.

list reply:
    ":(.+) 322 (.+) (#.+) (.+) :(.*)"
//:irc.troll.no 322 vrcats #quassel 1 :
//:irc.troll.no 322 vrcats #yber 11 :daily irc meeting @ 14 Finnish time, Monday irc meeting @ 11 Finnish time
//:irc.troll.no 323 vrcats :End of LIST

names reply:
    ":(.+) 353 (.*) ([+=@]) (#.+) :(.*)"
//:irc.troll.no 353 vrcats = #chinese :vrcats chayin_ julia_w7 ling lqi Jing jiang z35liu chayin
//:irc.troll.no 366 vrcats #chinese :End of NAMES list.

receive message:
    ":(.+)!~(.+) PRIVMSG (.+) :(.+)"
//:vrcats!~1@172.30.136.110 PRIVMSG vrcats :private message
//:z35liu!~z35liu@172.30.136.110 PRIVMSG #chinese :testing testing

pingpong message:
    "PING :(.*)"
//PING :irc.troll.no
//PONG :irc.troll.no

away reply:
    ":(.+) 306 (.+) :(.*)"
//:irc.troll.no 306 vrcats :You have been marked as being away

invitation:
    ":(.+)!~(.+) INVITE (.+) :(#.+)"
//:z35liu!~z35liu@172.30.136.110 INVITE vrcats :#chinese

invite reply:
    ":(.+) 341 (.+) (.+) (#.+)"
//:irc.troll.no 341 vrcats z35liu #vrcats

join message:
    ":(.+)!~(.+) JOIN :(#.+)"
//:z35liu!~z35liu@172.30.136.110 JOIN :#vrcats

};
*/
class IRCChannel
{
    public:
        IRCChannel(){}
        QStringList d_users;
        QString d_name;
};

class IRCClient: public QObject
{
    Q_OBJECT
    public:
        IRCClient(QString server, int port);
        void connect(QString& server, int port);
        void connect();
        QStringList channels();
        QStringList users(QString& channel);
        void join(QString& channel);
        void send(QString& channelOrUser);
        void away(QString& autoReplyMessage);
        void disconnect();
    signals:
        void message(QString from, QString fromURI, QString receiver, QString msg);
        void notification(QString msg);
        void invitation(QString from, QString toChannel);
        void connected();
        void ping(QString& ping);
        void join(QString username, QString channel, QString uri);
        void disconnected();
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
    private:
        QTcpSocket d_socket;
        QString d_server;
        int d_port;
    public:
        bool d_autoReconnect;
        QString d_userName, d_realName, d_password;
        int d_userMode;
        QString d_status;
        QList<IRCChannel> d_channels;
        QStringList d_motd;
};


#endif
