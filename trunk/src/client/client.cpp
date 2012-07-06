#include "client.h"

MyIM::MyIM(QWidget* parent):QWidget(parent)
{
    connect(&d_socket, SIGNAL(connected()), this, SLOT(registerMe()));
    connect(&d_socket, SIGNAL(readyRead()), this, SLOT(gotMsg()));
    connect(&d_socket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(stateChanged(QLocalSocket::LocalSocketState)));
}
MyIM::~MyIM()
{
    //unregister this app
    d_socket.disconnectFromServer();
}
void MyIM::connectServer()
{
    qDebug()<<"client: connecting to IMF server...";
    d_socket.connectToServer("IMFService");
}
void MyIM::registerMe()
{
    qDebug()<<"client: connected to server, now registering MyIM service...";
    qDebug()<<d_socket.state();
    d_socket.write("<msg protocol=IMFServer command=registerClient clientName=MyIM>please register me as chess client</msg>\r\n");
    d_socket.flush();
}
void MyIM::sendMsg(QString msg)
{
    msg="<msg protocol= app=MyIM threadId=1>" + msg + "</msg>\r\n";
    d_socket.write(msg.toUtf8());
}
void MyIM::gotMsg()
{
    QString str(d_socket.readAll());
    qDebug()<<"client: got msg from server:";
    qDebug()<<str;
}
