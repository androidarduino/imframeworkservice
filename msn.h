#ifndef MSN_H
#define MSN_H
#include "md5.h"
#include "challenge.h"
#include "msnauth.h"
#include "msnsession.h"
#include "msncontact.h"
#include "contactlist.h"
#include "switchboard.h"
#include <QString>
#include <QMap>
#include <QStringList>
#include <QList>
#include <QUrl>

class SwitchBoardSession;

class Msn:public QObject
{
    Q_OBJECT
    public:
        Msn ();
        virtual ~ Msn ();
        void setUsernamePassword (QString username, QString password);
        void setStatus (QString status);
        void sendMsg (QString receiver, QString message);
        void addContact (QString contact, QString group);
        void removeContact (QString contact);
        void blockContact (QString contact);
        void unblockContact (QString contact);
        QMap < QString, QString > getProfile (MsnContact contact);
        QList < MsnContact * >getContacts (QString status);
        QList < QString > getContactsEmails (QString status);
        QList < SwitchBoardSession * >m_sessions;	//active sessions
        SwitchBoardSession *requestSBSession (QString receiver);
        MsnContact *findContact (QString type, QString value);

    signals:
        void msgReceived (QString message, QString sender,
                SwitchBoardSession * session);
        void rawMsgReceived (QString message, QString sender,
                SwitchBoardSession * session);
        void invitedToSession (MsnContact invitor, SwitchBoardSession * session);
        void contactOnline (MsnContact contact);
        void contactOffline (MsnContact contact);
        void contactStatusChanged (MsnContact contact, QString status);
        void beingAddedBy (MsnContact contact);
        void userJoinSession (SwitchBoardSession * sb, QString email);
        void userLeftSession (SwitchBoardSession * sb, QString email);
        void loginFinish ();
        void syncSession ();
        void contactListChanged ();
        void userTyping (QString email);
        void sessionTyping (SwitchBoardSession * sb);

    private:
        QString m_username, m_password;
        int skipBytes;
        MsnContactList* buddyList;
        MsnSession *loginSession;
        MsnAuth *auth;
        QMap < QString, QString > profile;
        QList < MsnContact * >m_contacts;	//all contacts
        QMap < QString, int >m_contact_groups;	//contact groups
        QMap < QString, QString > statusConverter;

        private slots:
            void loginStage2 (QString ticket);	//called by login()
        void replyChallenge (QString cmd);	//reply to the server challenge string
        void distributeCmd (QString incomingCommand);	//command distributor
        void acceptInvitation (QString cmd);	//accept the invitation and make a switchboard session
        void connectionLost ();
        void userJoinLeftConversation (SwitchBoardSession * sb, QString email,
                QString reason);
        void loginStage3();
    public slots:
        bool loginMsn ();	//login to msn server
        void removeSB (SwitchBoardSession * session);
};

#endif
