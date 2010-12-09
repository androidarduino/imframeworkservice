#include <QApplication>
#include "adapter.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    /*IMAccount account;
    account.accountName="irc";
    account.type="irc";
    account.userName="vrcatsliu";
    account.password="";
    account.server="irc.freenode.net";
    account.port="6667";
    account.groups="#chinese,#liu";
    account.memo="";
    IRCIMClient client(account);
    client.login();
    */
    IMService service;
    return app.exec();
}
