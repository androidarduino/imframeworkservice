#include "msnsession.h"
#include <QDebug>
#include <QUrl>
#include <QTextCodec>
MsnSession::MsnSession (QString server, int port)	//constructor
{
  init (server, port);
}

MsnSession::MsnSession (QString serverAndPort)	//another form of constructor for convenience
{
  QString server = serverAndPort.mid (0, serverAndPort.indexOf (":"));
  QString port = serverAndPort.mid (serverAndPort.indexOf (":") + 1, -1);
  init (server, port.toInt ());
  sbServer = server;
  sbPort = port.toInt ();
}

int
MsnSession::state ()
{
  return m_socket->state ();
}

bool
MsnSession::init (QString server, int port)
{
  m_index = 1;
  m_socket = new QTcpSocket ();
  m_socket->connectToHost (server, port, QIODevice::ReadWrite);
  if (!m_socket->waitForConnected (4000))
    {
      qDebug () << "Unable to connect to server";
      return false;
    }
  connect (m_socket, SIGNAL (readyRead ()), this, SLOT (received ()));
  connect (m_socket, SIGNAL (disconnected ()), this,
	   SIGNAL (disconnected ()));
  return true;
}

int
MsnSession::sendCommand (const char *cmd, QString p1, QString p2, QString p3, QString p4)	//send a command to the server, put %x in the cmd to be replaced by the command index and p1, p2...
{
  qDebug()<<cmd;
  char cmdBuffer[2000];
  sprintf (cmdBuffer, cmd, m_index, qPrintable (p1), qPrintable (p2),
	   qPrintable (p3), qPrintable (p4));
  QByteArray question (cmdBuffer);
  QString o = question;
  qDebug () << ">> " << o.replace ("\r\n", "\\r\\n");
  if (m_socket->state () != QAbstractSocket::ConnectedState)
    {
      if (!init (sbServer, sbPort))	//try to connect again
	return 0;
    }
  m_socket->write (question);
  m_index++;
  qDebug () << "send command finished....";
  return m_index - 1;
}

QString MsnSession::getAnswer (int cmdIndex, int timeOut)	//get an answer with cmdIndex, if not found, wait for it. if the answer doesn't arrive until timeout, return "TimeOut"
{
  if (m_socket->state () != QAbstractSocket::ConnectedState)
    {
      qDebug () << "getting answer... not connected....";
      if (!init (sbServer, sbPort))	//try to connect again
	return "Not Connectted to Server";
    }
  QString found = "TimeOut";
  while (found == "TimeOut")
    {
      QString ans;
      foreach (ans, m_answers)
      {
        if(findReg("[A-Z]{3} (\\d+ )", ans, 1).toInt() ==cmdIndex)
//	if (ans.split (" ").at (1).toInt () == cmdIndex)
	  {
	    found = ans;
	  }
      }
      m_socket->waitForReadyRead (timeOut);
    }
  QString o = found;
  qDebug () << "<< " << o.replace ("\r\n", "\\r\\n");
  return found;
}

void
MsnSession::received ()
{
  QByteArray a = m_socket->readAll ();
  QString answer = QString::fromUtf8 (a.constData ());
  QString tmp = answer.left (3);
  bool isPayloadCmd;
  if((tmp == "MSG") | (tmp == "NOT") | (tmp == "PAG") | (tmp == "QRY"))
    isPayloadCmd = true;
  else
    isPayloadCmd = false;
  while ((!isPayloadCmd) && (a.right (2) != "\r\n"))	//if the command does not end with \r\n, that means it is incomplete, wait for a second for the rest part of the command to arrive TODO: this is not tested yet.
    {
      if (m_socket->state () != QAbstractSocket::ConnectedState)
	{
	  if (!init (sbServer, sbPort))
	    return;
	}
      m_socket->waitForReadyRead (1000);
      a = m_socket->readAll ();
      answer = answer + QString::fromUtf8 (a.constData ());
    }
  m_answers << answer;
  QStringList cmds = answer.split ("\r\n", QString::SkipEmptyParts);
  QString cmd, payloadCmd;
  int countDown = 0;
  foreach (cmd, cmds)
  {
    QString o = cmd;
    qDebug () << "<< " << o.replace ("\r\n", "\\r\\n");
    //if the cmd is a payload cmd, join it with its payload
    tmp = cmd.left (3);
    if ((tmp == "MSG") | (tmp == "NOT") | (tmp == "PAG") |(tmp == "QRY"))
      {
	int pos = cmd.lastIndexOf (" ");
	pos++;
	countDown = cmd.mid (pos, -1).toInt () - 2;	//because the \r\n following the first line is count in payload length
	payloadCmd = cmd;
	continue;
      }
    //if this line is a follow up of the payload cmd, merge them
    if (countDown > 0)
      {
	payloadCmd = payloadCmd + "\r\n" + cmd;
	countDown -= cmd.toUtf8 ().length () + 2;	//convert it back to utf8 to check its real length
	if (countDown <= 0)
	  emit incomingMsg (payloadCmd);
      }
    else
      {
	emit incomingMsg (cmd);
      }
    //qDebug()<<"countdown:"<<countDown;
    mspAuth=findReg("MSPAuth: (\\S+)", answer, 1);
  }
}

QString
MsnSession::findReg (QString regExp, QString findIn, int whichText)
{
  QRegExp rx (regExp);
  if (rx.indexIn (findIn) == -1)
    return "NotFound";
  return rx.cap (whichText);
}

QString
  MsnSession::sendAndWait (const char *cmd, QString p1, QString p2,
			   QString p3, QString p4)
{
  int retNumber;
  retNumber = sendCommand (cmd, p1, p2, p3, p4);
  return getAnswer(retNumber, 100);
}
