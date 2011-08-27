#include "adapter.h"
#include <QSettings>

IMService::IMService()
{
    /*process the start up things
      load accounts, find available clients,
      start the clients as threads,
      login them, and monitor their status,
      */
    //TODO: make sure only one instance is running
    QSettings accounts("accounts.ini", QSettings::IniFormat);
    accounts.sync();
    QStringList allAccounts=accounts.value("Accounts/allAccounts").toString().split(",");
    qDebug()<<allAccounts;
    //for each account, load details
    foreach(QString s, allAccounts)
    {
        IMAccount a;
        a.accountName=s.trimmed();
        s=s.trimmed()+"/%1";
        a.type=accounts.value(s.arg("type")).toString();
        a.userName=accounts.value(s.arg("username")).toString();
        a.password=accounts.value(s.arg("password")).toString();
        a.server=accounts.value(s.arg("server")).toString();
        a.port=accounts.value(s.arg("port")).toString();
        a.groups=accounts.value(s.arg("group")).toString();
        a.memo=accounts.value(s.arg("memo")).toString();
        a.candos=accounts.value(s.arg("candos")).toString();
        qDebug()<<a.type<<a.userName<<a.password<<a.server<<a.port<<a.groups<<a.memo;
        //add them to the list
        d_accounts<<a;
    }
    d_id=0;
}

void IMService::start()
{
    //create a client for each account
    //login all accounts
    foreach(IMAccount account, d_accounts)
    {
        IMClient* client=0;
        //check type and create client accordingly
        if(account.type.toLower()=="irc")
            client=new IRCIMClient(account);
        if(account.type.toLower()=="xmpp")
            client=new XMPPIMClient(account);
//        if(account.type.toLower()=="msn")
//            client=new MSNIMClient(account);
        if(client==0)
            continue;
        client->login();
        d_clients<<client;
        connect(client, SIGNAL(gotMsg(QString, QString)), this, SLOT(receivedMsg(QString, QString)));
        connect(client, SIGNAL(updated()), this, SIGNAL(updated()));
    }
    //TODO: setup socket service for interface classes access
}

void IMService::stop()
{
    //logout all clients
    //delete them (maybe later?)
    foreach(IMClient* client, d_clients)
        client->logout();
    d_clients.clear();
}

long long IMService::sendMsg(QString target, QString app, QString message, IMClient* client)
{
    qDebug()<<"service sending: "<<target<<app<<message;
    //search all the client accounts for best delivery method
    //send out the message to target, returns a receipt number
    IMClient* c=0;
    if(client==0)
        foreach(c, d_clients)
        {
            c->update();
            if(c->hasUser(target))
                break;
        }
    else
        c=client;
    QString msg="%1:%2 %3";
    msg=msg.arg(++d_id).arg(app).arg(message);
    if(c==0)
        return -1;
    qDebug()<<"sending over:"<<c->name();
    c->sendMsg(target, msg);
    return d_id;
}

long long IMService::sendMsg(QStringList targets, QString app, QString message, IMClient* client)
{
    //search all the client accounts for best delivery method
    //send out the message to target, returns a receipt number
    qDebug()<<"service sending: "<<targets<<app<<message;
    d_id++;
    foreach(QString target, targets)
    {
        IMClient* c=0;
        if(client==0)
            foreach(c, d_clients)
                if(c->onlineBuddies.contains(target))
                    break;
        else
            c=client;
        QString msg="%1:%2 %3";
        msg=msg.arg(d_id).arg(app).arg(message);
        if(c!=0)
            c->sendMsg(target, msg);
    }
    return d_id;
}

void IMService::receivedMsg(QString from, QString message)
{
    /*
    In this function, the incoming message from various backend will be processed.
    The message will get dispatched, distilling the target resource, and sent to
    the resource consequently.
     */
     qDebug()<<"got message from: "<<from<<":" <<message;
     //create a regular expression for "msg_id:target msg" and capture the parameters to target, msg
     //temporarily simply emit a message to all clients
     //in the long term, individual msg should be sent to their target application only through socket protocol
     emit gotMsg(from, "application", message, 999);
}

QString IMService::presence(QString uri)
{
    //TODO: provide more details for the presence
    foreach(IMClient* c, d_clients)
        if(c->onlineBuddies.contains(uri))
            return "Online";
    return "";
}

QStringList IMService::friends(IMClient* client)
{
    if(client!=0)
        return client->getPresence();
    //client==0 means all clients
    QStringList f;
    foreach(IMClient* c, d_clients)
        f<<c->getPresence();
    return f;
}

QList<IMClient*>& IMService::clients()
{
    return d_clients;
}

IMService::~IMService()
{
    foreach(IMClient* client, d_clients)
        delete client;
}
