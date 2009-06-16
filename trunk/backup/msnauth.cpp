#include "msnauth.h"
#include <QDebug>
MsnAuth::MsnAuth(QString userName, QString password, QString ticket)
{
  m_authString = "";
  m_username=userName;
  m_password=password;
  m_ticket=ticket;
  connect(&https, SIGNAL(requestFinished(int, bool)), this, SLOT(reply(int, bool)));
  qDebug()<<"authentication object initialized";
}

void MsnAuth::authenticate()
{
    qDebug()<<"starting authentication......";
    QFile file("soap.xml");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data=file.readAll();
    file.close();
    data.replace("msnp13_auth_ticket", m_ticket);
    data.replace("msnp13_auth_username", m_username);
    data.replace("msnp13_auth_password", m_password);
    qDebug()<<"posting data to auth server: "<<data;
    https.setHost("loginnet.passport.com", QHttp::ConnectionModeHttps, 443);
    https.post("/RST.srf", data.toAscii());
}

void MsnAuth::reply(int, bool)
{
//TODO: process error here
    QString ret;
    qDebug()<<"server returns: "<<(ret=https.readAll());
    QRegExp exp(">(t=[^<]+)");
    if(exp.indexIn(ret)==-1)
        return;
    QString authKey=exp.cap(1);
    //authKey=QString(QByteArray::fromPercentEncoding(authKey.toAscii()));
    authKey=authKey.replace(";p=","&p=");
    qDebug()<<"authentication key: "<<authKey;
    emit authCompleted(authKey);
}
