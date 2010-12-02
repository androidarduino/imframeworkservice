#include "adapter.h"

IMService::IMService()
{

}

IMService::~IMService()
{

}

IMClient::IMClient(IMAccount& account)
{
    d_accountName=account.accountName;
    d_userName=account.userName;
    d_password=account.password;
    d_server=account.server;
    d_port=account.port;
    d_groups=account.groups;
    d_memo=account.memo;
}

IMClient::~IMClient()
{

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
}

void IRCIMClient::gotIRCMessage(QString from, QString /*fromURI*/, QString /*receiver*/, QString msg)
{
    emit(gotMsg(from, msg));
}

IRCIMClient::~IRCIMClient()
{
    delete client;
    //disconnect the signals and slots
}

void IRCIMClient::login()
{
    client->connect();
    qDebug()<<"logging in...";
    QStringList channels=d_groups.split(",");
    foreach(QString channel, channels)
        client->join(channel);
}

void IRCIMClient::logout()
{
    client->disconnect();
}

void IRCIMClient::sendMsg(QString target, QString message)
{
    client->send(target, message);
}

QStringList IRCIMClient::getPresence()
{
    QStringList channels=client->channels();
    QStringList ret;
    foreach(QString channel, channels)
    {
        ret<<client->users(channel);
    }
    foreach(QString r, ret)
        r+=":Online";//there is no querable status on IRC, so we mark everyone "Online".
    return ret;
}


