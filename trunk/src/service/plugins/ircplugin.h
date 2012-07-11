#ifndef IRC_PLUGIN_H
#define IRC_PLUGIN_H

#include "../protocolinterface.h"
#include <QDebug>
#include <QString>

class IMIRCPlugin: public QObject, public IMProtocol
{
    Q_OBJECT
    Q_INTERFACES(IMProtocol);
    public:
        IMIRCPlugin(){}
        ~IMIRCPlugin(){}
        bool available(){return true;}
        QList<Msg> onlineBuddies(){return *(new QList<Msg>());}
        QString& operator[](QString){return *(new QString("Yeah"));}
        void sendMsg(Msg&msg){qDebug()<<"IRC protocol is sending: ";}
        void login();
        QString test(){return QString("test successful");}
};

#endif
