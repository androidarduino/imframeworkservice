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
        qDebug()<<a.type<<a.userName<<a.password<<a.server<<a.port<<a.groups<<a.memo;
        //add them to the list
        d_accounts<<a;
    }
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
//        if(account.type.toLower()=="xmpp")
//            client=new XMPPIMClient(account);
//        if(account.type.toLower()=="msn")
//            client=new MSNIMClient(account);
        if(client==0)
            continue;
        client->login();
        d_clients<<client;
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

long IMService::sendMsg(QString target, QString message, IMClient* client)
{
    //search all the client accounts for best delivery method
    //send out the message to target, returns a receipt number
}

 