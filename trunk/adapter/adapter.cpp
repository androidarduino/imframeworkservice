#include "adapter.h"

IMClient::IMClient(IMAccount& account)
{
    d_accountName=account.accountName;
    d_userName=account.userName;
    d_password=account.password;
    d_server=account.server;
    d_port=account.port;
    d_groups=account.groups;
    d_memo=account.memo;
    connect(this, SIGNAL(connected()), this, SLOT(update()));
    connect(this, SIGNAL(disconnected()), this, SLOT(offline()));
    available=false;
}

IMClient::~IMClient()
{

}

void IMClient::offline()
{
    available=false;
    onlineBuddies.clear();
}

void IMClient::update()
{
    available=true;
    onlineBuddies=getPresence();
    emit updated();
}

bool IMClient::canDo(QString )
{
    return false;//for each concrete class, provide according abilities
}

IRCIMClient::IRCIMClient(IMAccount& account):IMClient(account)
{
    client=new IRCClient(d_server, d_port.toInt());
    client->d_userName=account.userName;
    if(client->d_realName=="")
        client->d_realName=account.userName;
    //connect the signals and slots
    QObject::connect(client, SIGNAL(connected()), this, SIGNAL(connected()));
    QObject::connect(client, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    QObject::connect(client, SIGNAL(message(QString, QString, QString, QString)), this, SLOT(gotIRCMessage(QString, QString, QString, QString)));
    QObject::connect(client, SIGNAL(updated()), this, SLOT(update()));
}

XMPPIMClient::XMPPIMClient(IMAccount& account):IMClient(account)
{
    client=new QXmppClient();
    //QXmppLogger::getLogger()->setLoggingType(QXmppLogger::StdoutLogging);
    //connect the signals and slots
    QObject::connect(client, SIGNAL(connected()), this, SIGNAL(connected()));
    QObject::connect(client, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    QObject::connect(client, SIGNAL(messageReceived(const QXmppMessage &)), this, SLOT(gotXmppMessage(const QXmppMessage &)));
}

void IRCIMClient::update()
{
    onlineBuddies=client->users();
    qDebug()<<"IRC Client updated, online users:"<<onlineBuddies;
    emit updated();
}

void IRCIMClient::gotIRCMessage(QString from, QString /*fromURI*/, QString /*receiver*/, QString msg)
{
    emit(gotMsg(from, msg));
}

void XMPPIMClient::gotXmppMessage(const QXmppMessage& message)
{
    emit(gotMsg(message.from(), message.body()));
}


IRCIMClient::~IRCIMClient()
{
    delete client;
    //disconnect the signals and slots?
}

XMPPIMClient::~XMPPIMClient()
{
    delete client;
    //disconnect the signals and slots?
}

void IRCIMClient::login()
{
    client->connect();
    qDebug()<<"logging in IRC...";
    QStringList channels=d_groups.split(",");
    foreach(QString channel, channels)
        client->join(channel);
}

void XMPPIMClient::login()
{
    client->connectToServer(d_server, d_userName, d_password, d_port);
    qDebug()<<"logging in XMPP...";
}


void IRCIMClient::logout()
{
    client->disconnect();
}

void XMPPIMClient::logout()
{
    client->disconnectFromServer();
}

void IRCIMClient::sendMsg(QString target, QString message)
{
    client->send(target, message);
}

void XMPPIMClient::sendMsg(QString target, QString message)
{
    client->sendMessage(target, message);
}

QStringList IRCIMClient::getPresence()
{
    return client->users();
}

QStringList XMPPIMClient::getPresence()
{
    client->rosterManager().getRosterBareJids();
    return QStringList();
}


