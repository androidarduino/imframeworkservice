#include "server.h"

Msg::Msg(QByteArray& msg)
{
    //<msg a=a1 b=b1 c=c1>msgbody</msg>
    QRegExp rxMain("\\s*<\\s*msg\\s+([^>]*)>([^<]*)<\\s*/msg\\s*>\\s*");
    QRegExp rxParameters("\\s*(\\w*)=(\\w*)\\s*");
    if(rxMain.indexIn(QString(msg))==-1)
        return;
    QString body=rxMain.cap(2);
    QString parameters=rxMain.cap(1);
    int pos=0;
    while(pos=rxParameters.indexIn(parameters)==-1)
    {
        QString name=rx.cap(1);
        QString value=rx.cap(2);
        items[name]=value;
        pos+=rxParameters.matchedLength();
    }
}

QString& operator [](QString& name)
{
    return items[name];
}

