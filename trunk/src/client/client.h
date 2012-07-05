#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QLocalSocket>
#include <QStringList>
#include <QWidget>
#include "../msg.h"

class MyIM: public QWidget
{
    Q_OBJECT
    public:
        MyIM(QWidget* parent=0);
        ~MyIM();
        void connectServer();//connect to IMF server
        void sendMsg(QString msg);//send an message to the other side
    public slots:
        void gotMsg();//slot called when there is an incoming message
        void registerMe();//register this application
        void    stateChanged ( QLocalSocket::LocalSocketState){qDebug()<<d_socket.state();}
    public:
        QLocalSocket d_socket;
};

#endif
