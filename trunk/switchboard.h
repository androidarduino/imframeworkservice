#ifndef SWITCHBOARD_H
#define SWITCHBOARD_H
#include <QString>
#include <QRegExp>
#include <QPair>
#include "msnsession.h"
#include "msn.h"
class Msn;
class SwitchBoardSession:public QObject
{
Q_OBJECT public:
  SwitchBoardSession (QStringList oldHistory, QString username, QString invitationCmd);	//init a session from an invitation
  SwitchBoardSession (MsnSession * loginSession, QStringList receivers);
  bool inviteToThisSession (QString email);
  void sendMessage (QString receiver, QString message);
  bool talkingTo (QString email);
  QString getHistory (int lineCount = 10);
  QString getParticipants (int desiredLength, Msn * msn);
  void reConnect ();
  static QString toShort (QString input, int length);
  QStringList m_history;	//store the history messages on this session
  QStringList allParticipants ();

private:
    MsnSession * sbSession;
  MsnSession *m_loginSession;
  QString m_username;
  QStringList participants, left_participants;	//we can not use msn contacts here because some of the participants may not be in our list
  QString sessionId;
    QList < QPair < QString, QString > >msgQueue;
  void connectSB (QStringList receivers);
  void trySend (QString user);
  void send (QString receiver, QString message);
  private slots:void close ();
  void msgDistributor (QString inMsg);
  void addHistory (QString sender, QString fromNick, QString payloadLength,
		   QString payloadBody);
    signals:void receivedMsg (QString fromUser, QString fromNick,
			      QString payloadLength, QString payloadBody);
  void leftConversation (SwitchBoardSession * sb, QString user,
			 QString reason);
  void joinConversation (SwitchBoardSession * sb, QString user,
			 QString reason);
  void closed (SwitchBoardSession * me);
  void msgReceived (QString message, QString sender,
		    SwitchBoardSession * session);
  void rawMsgReceived (QString message, QString sender,
		       SwitchBoardSession * session);
  void userTyping (QString user);
  void sessionTyping (SwitchBoardSession * sb);
};
#endif
