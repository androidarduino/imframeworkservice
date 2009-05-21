#ifndef MSNAUTH_H
#define MSNAUTH_H
#include <qtopiacomm/QtopiaHttp>
#include <QSslSocket>
#include <unistd.h>

class MsnAuth:public QObject
{
Q_OBJECT public:
    MsnAuth (QString serverAndPath, QString certsPath);
  void authenticate (QString username, QString password, QString ticket);
  private slots:void loginServerArrived (const QHttpResponseHeader &
					 response);
  void authInfoArrived (const QHttpResponseHeader & response);
    signals:void authCompleted (QString authenticationString);
private:
    QSslSocket * m_sck1;
  QSslSocket *m_sck2;
  QtopiaHttp *http1;
  QtopiaHttp *http2;
  QString m_ticket, m_username, m_password, m_authString, m_serverAndPath;
};

#endif
