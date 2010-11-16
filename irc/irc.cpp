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
    //now we keep it as simple as possible. in the future it should record the title of channels and head counts as well.
    if(!d_channels.contains(cn))
        d_channels<<cn;
    qDebug()<<"--- got channel list, updating channels ";
}
void IRCClient::gotUserNames(QString& )
{
    return;
}
void IRCClient::gotNotification(QString& )
{
    return;
}
void IRCClient::gotInvitation(QString& )
{
    return;
}
void IRCClient::gotMessageOfTheDay(QString& )
{
    return;
}
void IRCClient::gotJoin(QString& )
{
    return;
}
