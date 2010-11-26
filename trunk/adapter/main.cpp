#include <QApplication>
#include "adapter.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    IMAccount account;
    account.accountName="irc";
    account.type="irc";
    account.userName="vrcats";
    account.password="";
    account.server="irc.nokia.troll.no";
    account.port="6667";
    account.groups="#chinese,#liu";
    account.memo="";
    IRCIMClient client(account);
    client.login();
    return app.exec();
}
