#include "irc.h"
#include <QApplication>

using namespace irc;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    IRCClient client("irc.nokia.troll.no", 6667);
    client.d_userName="vrcats";
    client.d_realName="zheng Liu";
    client.connect();
    client.join("#chinese");
    return app.exec();
}

