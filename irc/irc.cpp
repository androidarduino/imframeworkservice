#include "irc.h"

IRCClient::IRCClient(QString server, int port)
{
    QObject::connect(&d_socket, SIGNAL(connected()), this, SIGNAL(connected()));
    QObject::connect(&d_socket, SIGNAL(connected()), this, SLOT(login()));
    QObject::connect(&d_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    QObject::connect(&d_socket, SIGNAL(readyRead()), this, SLOT(msgArrived()));
    d_server=server;
    d_port=port;
}

void IRCClient::msgArrived()
{
    QString data;
    do
    {
        data=QString(d_socket.readLine(5000));
        qDebug()<<"<<"<<data;
        //dispatch commands
        gotPing(data);
        gotMsg(data);
        gotChannelList(data);
        gotUserNames(data);
        gotNotification(data);
        gotInvitation(data);
        gotMessageOfTheDay(data);
        gotJoin(data);
    }
    while(data!="");
}

void IRCClient::connect()
{
    d_socket.connectToHost(d_server, d_port);
}

void IRCClient::sendCommand(QString cmd)
{
    QByteArray command=cmd.toUtf8();
    d_socket.write(command+"\n");
    qDebug()<<">>"<<command;
}

void IRCClient::login()
{
    sendCommand("user vrcats 0 * zhengliu");
    sendCommand("nick vrcats");
    sendCommand("list");
}

void IRCClient::gotPing(QString& msg)
{
    QRegExp rx("PING :(.*)");
    if(rx.indexIn(msg)==-1)
        return;
    sendCommand(QString("PONG %1").arg(rx.cap(1)));
    qDebug()<<"--- got ping command, pong back ";
}
void IRCClient::gotMsg(QString& msg)
{
    QRegExp rx(":(.+)!~(.+) PRIVMSG (.+) :(.+)");
    if(rx.indexIn(msg)==-1)
        return;
    emit(message(rx.cap(1), rx.cap(2), rx.cap(3), rx.cap(4)));
    qDebug()<<"--- got message, emitting msg signal ";
}
void IRCClient::gotChannelList(QString& msg)
{
    QRegExp rx(":(.+) 322 (.+) ([#&].+) (.+) :(.*)");
    if(rx.indexIn(msg)==-1)
        return;
    QString cn=rx.cap(3);
    //add channel to list
    bool exist=false;
    foreach(IRCChannel c, d_channels)
        if(c.d_name==cn)
            exist=true;
    if(!exist)
    {
        IRCChannel* channel=new IRCChannel();
        channel->d_name=cn;
        d_channels<<*channel;
    }
    qDebug()<<"--- got channel list, updating channels ";
}
void IRCClient::gotUserNames(QString& msg)
{
    QRegExp rx(":(.+) 353 (.*) ([+=@]) (#.+) :(.*)");
    if(rx.indexIn(msg)==-1)
        return;
    QString channel=rx.cap(4);
    QStringList users=rx.cap(5).split(" ");
    //add user to channel
    foreach(IRCChannel c, d_channels)
        if(c.d_name==channel)
            foreach(QString user, users)
                if(!c.d_users.contains(user))
                    c.d_users<<user;
    qDebug()<<"--- got user list, updating channel users ";
   return;
}
void IRCClient::gotNotification(QString& msg)
{
    QRegExp rx(":(.+) NOTICE (.+) :(.*)");
    if(rx.indexIn(msg)==-1)
        return;
    emit(notification(rx.cap(3)));
    qDebug()<<"--- got notification, emitting notify signal ";
}
void IRCClient::gotInvitation(QString& msg)
{
    QRegExp rx(":(.+)!~(.+) INVITE (.+) :(#.+)");
    if(rx.indexIn(msg)==-1)
        return;
    emit(invitation(rx.cap(1), rx.cap(4)));
    sendCommand("join "+rx.cap(4));
    qDebug()<<"--- got invitation, emitting invite signal ";
}
void IRCClient::gotMessageOfTheDay(QString& msg)
{
    QRegExp rx(":(.+) 372 (.+) :(.*)");
    if(rx.indexIn(msg)==-1)
        return;
    if(!d_motd.contains(rx.cap(3)))
            d_motd<<rx.cap(3);
    qDebug()<<"--- got message of the day, updating ";
}
void IRCClient::gotJoin(QString& msg)
{
    QRegExp rx(":(.+)!~(.+) JOIN :(#.+)");
    if(rx.indexIn(msg)==-1)
        return;
    emit(join(rx.cap(1), rx.cap(3), rx.cap(2)));
    qDebug()<<"--- got join notification, emitting join signal ";
}
