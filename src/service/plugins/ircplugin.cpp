#include "ircplugin.h"

void IMIRCPlugin::login()
{
    qDebug()<<"irc logging in...";
}

Q_EXPORT_PLUGIN2(imircplugin, IMIRCPlugin);
