#include "switchboard.h"
#include "mime.h"
#include <QDebug>
#include "emotions.h"

SwitchBoardSession::SwitchBoardSession (QStringList oldHistory,
					QString username,
					QString invitationCmd)
{
//answer to a SB session call
//parse the command and initialize the switch board
//sample invitation:
//RNG 351674855 207.46.27.24:1863 CKI 1932219.39246180 robot_liuzheng@hotmail.com (R)
//open a new connection to the sb server provided, and send ANS within two minutes
//then get the participants list by IRO command
  QRegExp rx ("RNG (\\S+) (\\S+) CKI (\\S+) (\\S+) (\\S+)");
  rx.indexIn (invitationCmd);
  sessionId = rx.cap (1);
  QString sbServer = rx.cap (2);
  QString authString = rx.cap (3);
  QString invitor = rx.cap (4);
  QString invitorNickName = rx.cap (5);
  sbSession = new MsnSession (sbServer);
  connect (sbSession, SIGNAL (incomingMsg (QString)), this,
	   SLOT (msgDistributor (QString)));
  sbSession->sendAndWait ("ANS %d %s %s %s\r\n", username, authString,
			  sessionId);
  //if the connection is closed, emit close signal
  connect (sbSession, SIGNAL (disconnected ()), this, SLOT (close ()));
  connect (this, SIGNAL (receivedMsg (QString, QString, QString, QString)),
	   this, SLOT (addHistory (QString, QString, QString, QString)));
  m_history = oldHistory;
}

SwitchBoardSession::SwitchBoardSession (MsnSession * loginSession,
					QStringList receivers)
{
  m_loginSession = loginSession;
  m_username = receivers[0];
  sbSession = NULL;
  connectSB (receivers);
}

QStringList SwitchBoardSession::allParticipants ()
{
  QStringList
    l;
  l << participants;
  l << left_participants;
  return l;
}

void SwitchBoardSession::reConnect ()
{
  //re-invite all participants who have left
  QStringList list;
  list << m_username;
  list << left_participants;
  connectSB (list);
}

void SwitchBoardSession::connectSB (QStringList receivers)
{
  //check if sbSession is initialized and connected, if not, request a new session
  //qDebug()<<"reconnecting to: "<<receivers;
  if ((sbSession == NULL)
      || (sbSession->state () != QAbstractSocket::ConnectedState))
    {
      QString XFR_Cmd = m_loginSession->sendAndWait ("XFR %d SB\r\n");
      QRegExp rx ("XFR \\d+ SB (\\S+) CKI ([\\d|\\.]+)");
      rx.indexIn (XFR_Cmd);
      QString sbServer = rx.cap (1);
      QString authString = rx.cap (2);
      sbSession = new MsnSession (sbServer);
      connect (sbSession, SIGNAL (incomingMsg (QString)), this,
	       SLOT (msgDistributor (QString)));
      connect (sbSession, SIGNAL (disconnected ()), this, SLOT (close ()));
      sbSession->sendAndWait ("USR %d %s %s\r\n", m_username, authString);
    }
  //invite all participants
  for (int i = 1; i < receivers.count (); i++)
    {
      inviteToThisSession (receivers[i]);
    }
}

void SwitchBoardSession::msgDistributor (QString cmd)
{
  //qDebug()<<"Receive SB commend: "<<cmd;
  QStringList cut = cmd.split (" ");
  if (cut[0] == cmd)
    return;
  QString prefix = cut[0];
  if (prefix == "IRO")
    {
      //IRO 1 1 1 robot_liuzheng@hotmail.com (R)
      //at this stage, ignore the first two parameters
      //just add the participant to the list
      participants << cut[4];
      //qDebug()<<"The following paticipants are in this sb session:"<<cut[4]<<cut[5];
    }
  if (prefix == "MSG")
    {
      //qDebug()<<"received a message! "<<cmd;
      QRegExp rx ("MSG (\\S+) (\\S+) (\\d+)\\r\\n(.+)");
      rx.indexIn (cmd);
      //qDebug()<<rx.cap(1)<<rx.cap(2)<<rx.cap(3)<<rx.cap(4);
      addHistory (rx.cap (1), rx.cap (2), rx.cap (3), rx.cap (4));
    }
  if (prefix == "BYE")		//a contact left the conversation. either conversation is closed or timeout, now only processing as closed
    {
      QRegExp rx ("BYE (\\S+)( \\d+)?");
      rx.indexIn (cmd);
      if (rx.cap (2) != "")
	{
	  //qDebug()<<"session closed by remote user";
	  emit leftConversation (this, rx.cap (1), "closed");
	}
      else
	emit leftConversation (this, rx.cap (1), "timeout");
      //qDebug()<<"The following contact has left this session: "<<rx.cap(1);
      QString email = rx.cap (1);
      for (int i = 0; i < participants.count (); i++)
	{
	  if (participants[i] == email)
	    {
	      left_participants << participants.takeAt (i);
	    }
	}
      //if(participants.isEmpty())
      //    emit closed(this);
      //it is weird that the participant seems to be leaving but will still be able to send me messages
      //so not closing session actively, waiting for server to disconnect after 5 minutes
    }
  if (prefix == "JOI")
    {
      QRegExp rx ("JOI (\\S+) ([^\\r]+)");
      rx.indexIn (cmd);
      participants << rx.cap (1);
      int idx = left_participants.indexOf (rx.cap (1));
      if (idx != -1)
	left_participants.removeAt (idx);
      //qDebug()<<"The following paticipants joined this sb session:"<<rx.cap(1);
      emit joinConversation (this, rx.cap (1), "join");
      trySend (rx.cap (1));
    }
}

bool SwitchBoardSession::inviteToThisSession (QString email)
{
  if (email == "")
    return false;
  QString
    ret = sbSession->sendAndWait ("CAL %d %s\r\n", email).left (3);
  if (ret != "CAL")
    {
      qDebug () << "Can not invite user to this switch board session.";
      return false;
    }
  return true;
}

void SwitchBoardSession::sendMessage (QString receiver, QString message)
{
  //if the participant is in this switch board session
  foreach (QString p, participants)
  {
    if (p == receiver)
      {
	send (receiver, message);
	return;
      }
  }
  //qDebug()<<"receiver not in session, pending: "<< receiver;
  msgQueue << QPair < QString, QString > (receiver, message);
}

void SwitchBoardSession::trySend (QString user)
{
  if (sbSession->state () != QAbstractSocket::ConnectedState)
    reConnect ();
  QPair < QString, QString > p;
  foreach (p, msgQueue)
  {
    if (p.first == user)
      {
	send (user, p.second);
      }
  }
}

void SwitchBoardSession::send (QString /*receiver */ , QString message)
{
  QString len;
  len.setNum (124 + message.length ());
  sbSession->
    sendCommand
    ("MSG %d N %s\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: FN=MS%%20Sans%%20Serif; EF=; CO=0; CS=0; PF=0\r\n\r\n%s",
     len, message);
  //we set it to type N so no acknowledgement will be received
//TODO: fix this 124
  addHistory ("me", "Me", 0, message);
}

void SwitchBoardSession::close ()
{
  //qDebug()<<"SB Session closing...";
  emit closed (this);
}

bool SwitchBoardSession::talkingTo (QString email)
{
  //qDebug()<<"exec here";
  if (participants.count () == 1 && participants[0] == email)
    return true;
  if (left_participants.count () == 1 && left_participants[0] == email)
    return true;
  return false;
}

QString SwitchBoardSession::getHistory (int lineCount)
{
  QStringList
    list;
  int
    count = m_history.count ();
  //qDebug()<<"line count: "<<count;
  for (int i = count - lineCount; i < count; i++)
    if (i >= 0)
      list << m_history[i];
  if (list.count () == 1)
    return list[0];
  return list.join ("<br><hr>") +
    "<br><a href=\"#\" name=\"bottom\"><hr></a>";
}

QString SwitchBoardSession::getParticipants (int /* desiredLength */ , Msn * msn)
{
  QStringList pt;
  pt << participants << left_participants;
  QString status;
  for (int i = 0; i < pt.count (); i++)
    {
      QString & p = pt[i];
      status = msn->findContact ("email", p)->get ("status");
      //qDebug()<<p<<": "<<status;
      if (status == "OFF")
	p = "<font color=#c0c0c0>" + p + "</font>";
      else if (status == "BSY")
	p = "<font color=#ff2020>" + p + "</font>";
      else if (status == "NLN")
	p = "<font color=#2020ff>" + p + "</font>";
      else
	p = "<font color=#0c0c0c>" + p + "</font>";
    }
  return pt.join (",");
}

QString SwitchBoardSession::toShort (QString input, int length)
{
  if (input.length () <= length)
    return input;
  return input.left (length - 3) + "...";
}

void SwitchBoardSession::addHistory (QString sender, QString fromNick,
				QString /*payloadLength */ ,
				QString payloadBody)
{
  qDebug () << "adding to history......." << payloadBody;
//    QRegExp rx("PF=\\d+\\r\\n(.+)");
//    rx.indexIn(payloadBody);
//    QString msg=rx.cap(1);
  Mime mime (payloadBody);
//    mime.print();
  QString msg = mime.getHtml ();
  if (mime.typingUser != "")
    {
      emit userTyping (mime.typingUser);
      emit sessionTyping (this);	//for the message dialog to use
    }
  if (msg == "")		//typing message or sthing
    return;
  emit rawMsgReceived (msg, sender, this);
  QString s = "<b>%1:</b> %2";
  fromNick = QUrl::fromPercentEncoding (fromNick.toUtf8 ());
  fromNick = toShort (fromNick, 20);
  s = s.arg (fromNick).arg (msg.replace ("\r\n", "<br>"));
  //add emtion rich texts
  QString emotionText, emotionPicture;
  QStringList emotions = Emotions::loadEmotions ();
  for (int i = 0; i < emotions.count (); i += 2)
    {
      emotionText = emotions[i];
      emotionPicture = emotions[i + 1];
      emotionText = emotionText.toLower ();
      s.replace (emotionText, "<img src=\":/pics/" + emotionPicture + "\">");
      s.replace (emotionText.toUpper (),
		 "<img src=\":/pics/" + emotionPicture + "\">");
    }
  m_history << s;
  qDebug () << "message added to history: " << s;
  emit msgReceived (s, sender, this);
}
