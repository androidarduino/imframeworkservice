#include "msnauth.h"
#include <QDebug>
MsnAuth::MsnAuth (QString serverAndPath, QString /*certsDir */ )
{
  m_serverAndPath = serverAndPath;
  m_authString = "";
  //  m_sck1 = new QtSslSocket (QtSslSocket::Client, 0);
  //  m_sck1->setPathToCACertDir (certsDir);
  //  m_sck2 = new QtSslSocket (QtSslSocket::Client, 0);
  //  m_sck2->setPathToCACertDir (certsDir);
  m_sck1 = new QSslSocket (this);
  m_sck2 = new QSslSocket (this);
  //qDebug()<<"authentication object initialized";
}

void
MsnAuth::authenticate (QString username, QString password, QString ticket)
{
  m_ticket = ticket;
  m_username = username;
  m_password = password;
  http1 = new QtopiaHttp ();
  http1->setSslSocket (m_sck1);
  connect (http1,
	   SIGNAL (responseHeaderReceived (const QHttpResponseHeader &)),
	   this, SLOT (loginServerArrived (const QHttpResponseHeader &)));
  http1->startFetch (QUrl (m_serverAndPath));
}

void
MsnAuth::loginServerArrived (const QHttpResponseHeader & response)
{
  QString answer = response.toString ();
  //qDebug()<<"Login server info arrived.....";
  QRegExp rx ("(DALogin=)([^,]+)");
  if (rx.indexIn (answer) == -1)
    qDebug () << "Error: incorrect response from server:\n" << answer;
  QString loginServer = "https://" + rx.cap (2);
  //qDebug()<<loginServer;
  http2 = new QtopiaHttp ();
  connect (http2,
	   SIGNAL (responseHeaderReceived (const QHttpResponseHeader &)),
	   this, SLOT (authInfoArrived (const QHttpResponseHeader &)));
  http2->setSslSocket (m_sck2);
  QString auth =
    "Passport1.4 OrgVerb=GET,OrgURL=http%3A%2F%2Fmessenger%2Emsn%2Ecom,sign-in="
    + m_username + ",pwd=" + m_password + "," + m_ticket;
  //qDebug()<<auth;
  http2->setAuthorization (auth);
  http2->startFetch (QUrl (loginServer));
  //qDebug()<<"Logging in to login server....";
}

void
MsnAuth::authInfoArrived (const QHttpResponseHeader & response)
{
  //qDebug()<<"auth info received ..."<<response.toString();
  QRegExp rx ("(authentication-info:[^']+')(t=\\S+)(')", Qt::CaseInsensitive);	//sometimes it returns "Authentication-Info..."
  if (rx.indexIn (response.toString ()) == -1)
    return;
  m_authString = rx.cap (2);
  //qDebug()<<m_authString;
  emit authCompleted (m_authString);
}
