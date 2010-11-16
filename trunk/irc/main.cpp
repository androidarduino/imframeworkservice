#include "irc.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    IRCClient client("irc.nokia.troll.no", 6667);
    client.d_userName="vrcats";
    client.connect();
    return app.exec();
}

