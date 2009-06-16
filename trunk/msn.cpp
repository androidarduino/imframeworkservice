#include <QDebug>
#include "msn.h"
Msn::Msn ()
{
    statusConverter["Online"] = "NLN";
    statusConverter["Busy"] = "BSY";
    statusConverter["Idle"] = "IDL";
    statusConverter["BeRightBack"] = "BRB";
    statusConverter["Away"] = "AWY";
    statusConverter["OnThePhone"] = "PHN";
    statusConverter["OutToLunch"] = "LUN";
    statusConverter["Hidden"] = "HDN";
    statusConverter["NLN"] = "Online";
    statusConverter["BSY"] = "Busy";
    statusConverter["IDL"] = "Idle";
    statusConverter["BRB"] = "BeRrightBack";
    statusConverter["AWY"] = "Away";
    statusConverter["PHN"] = "OnThePhone";
    statusConverter["LUN"] = "OutToLunch";
    statusConverter["HDN"] = "Hidden";
}

void Msn::setUsernamePassword (QString username, QString password)
{
    m_username = username;
    m_password = password;
}

Msn::~Msn ()
{
}

bool Msn::loginMsn ()
{
    QString answer, NSServer, authTicket;
    MsnSession s ("messenger.hotmail.com:1863");
    if (s.state () != QAbstractSocket::ConnectedState)
        return false;
    s.sendAndWait ("VER %d MSNP13 CVR0\r\n");
    s.sendAndWait ("CVR %d 0x0409 winnt 5.1 i386 MSNMSGR 8.0.0566 MSMSGS %s\r\n", m_username);
    answer = s.sendAndWait ("USR %d TWN I %s\r\n", m_username);
    NSServer = s.findReg ("(XFR \\d+ NS) (\\S+)", answer, 2);

    loginSession = new MsnSession (NSServer);
    connect (loginSession, SIGNAL (incomingMsg (QString)), this, SLOT (distributeCmd (QString)));
    connect (loginSession, SIGNAL (disconnected ()), this, SLOT (connectionLost ()));
    loginSession->sendAndWait ("VER %d MSNP13 CVR0\r\n");
    loginSession->sendAndWait ("CVR %d 0x0409 winnt 5.1 i386 MSNMSGR 8.0.0566 MSMSGS %s\r\n", m_username);
    answer = loginSession->sendAndWait ("USR %d TWN I %s\r\n", m_username);
    authTicket=loginSession->findReg ("(lc=1033\\S+)", answer, 1);
    authTicket="?"+authTicket.replace(",",";");
    authTicket=QString(QByteArray::fromPercentEncoding(authTicket.toAscii()));
    auth = new MsnAuth(m_username, m_password, authTicket);
    connect (auth, SIGNAL (authCompleted (QString)), this, SLOT (loginStage2 (QString)));
    auth->authenticate();
    return true;
}

void Msn::connectionLost ()
{
    qDebug () << "Connection has been closed by server";
    //loginMsn ();
}

    void
Msn::loginStage2 (QString ticket)
{
    QString answer;
    //qDebug()<<"Authenticate with ticket......................\n"<<ticket;
    answer = loginSession->sendAndWait ("USR %d TWN S %s\r\n", ticket);
    /*qDebug()<<answer;
    //grab properties into a QMap object
    QString propertyName, propertyValue, property;
    QStringList properties;
    properties << "(charset)=([^\\r\\n]+)"
    << "(MIME-Version): ([^\\r\\n]+)"
    << "(LoginTime): (\\d+)"
    << "(EmailEnabled): (\\d)+"
    << "(lang_preference): (\\d+)"
    << "(MSPAuth): (\\S+)" << "(ClientIP): (\\S+)" << "(ClientPort): (\\S+)";
    foreach (property, properties)
    {
    propertyValue = loginSession->findReg (property, answer, 2);
    propertyName = loginSession->findReg (property, answer, 1);
    profile.insert (propertyName, propertyValue);
    }
    //in msnp13, must retrive via soap
    //loginSession->sendAndWait ("SYN %d 0\r\n");
    //loginSession->sendAndWait ("CHG %d NLN\r\n");	//set init status here and next line
    */
    //qDebug()<<loginSession->mspAuth;
    //TODO: now populate in the contact list using soap
    buddyList=new MsnContactList(ticket.mid(2).replace("&p=", ";MSPProf="));
    connect(buddyList,SIGNAL(gotBuddyList()),this,SLOT(loginStage3())); 
    buddyList->requestMembershipList();
    buddyList->requestAddressBook();
}

void Msn::loginStage3()
{
    qDebug()<<"stage 3 started";
    //BLP
    loginSession->sendAndWait("BLP %d BL\r\n");
    //ADL
    QString ml=buddyList->ml();
    loginSession->sendAndWait("ADL %d %s\r\n%s",QString::number(ml.length()),ml);
    //PRP (if the address is not verified, rpr will return 715 7.
    //loginSession->sendAndWait("PRP %d MFN %s\r\n", QUrl::toPercentEncoding("vrcats@gmail.com"));
    //CHG
    loginSession->sendAndWait("CHG %d NLN %s\r\n", "1073791084");
    emit loginFinish();
}

void Msn::replyChallenge (QString incomingMsg)
{
    qDebug()<<"challenge received: "<<incomingMsg;
    QString seed = loginSession->findReg ("CHL 0 (\\d+)", incomingMsg, 1);
    QString clientId = "PROD01065C%ZFN6F";
    QString clientCode = "O4BG@C7BWLYQX?5G";	//magic code
//    QString tmp = seed + clientCode;	//conjunct two strings
//    tmp = MD5 (tmp).toString ();	//return md5 digest
    MsnChallenge c(clientId, clientCode);
    QString result=c.calculateChallenge(seed.toAscii().data());
    loginSession->sendCommand ("QRY %d %s 32\r\n%s", clientId, result);
}

void Msn::setStatus (QString status)
{
    loginSession->sendAndWait ("CHG %d %s\r\n", statusConverter[status]);	//internal use class, so no strict check required
}

QMap < QString, QString > Msn::getProfile (MsnContact /*contact */ )
{
    return profile;
}

void Msn::distributeCmd (QString cmd)
{
    //qDebug()<<"Receive command: "<<cmd;
    QStringList cut = cmd.split (" ");
    if (cut[0] == cmd)
        return;
    QString prefix = cut[0];
    if (prefix == "LSG")		//retrieve the list
    {
        //LSG groupid groupname
        m_contact_groups.insert (cut[1], cut[2].toInt ());
    }
    if (prefix == "LST")
    {
        //LST accountname nickname list groups
        //TODO: Should check if the contact is already there
        MsnContact *c = findContact ("email", cut[1]);
        int fields = cut.count ();	//some contacts may do not have all parameters
        c->set ("email", cut[1]);
        if (fields > 2)
            c->set ("nickname", cut[2]);
        if (fields > 3)
            c->set ("list", cut[3]);
        if (fields > 4)
            c->set ("group", cut[4]);
        c->set ("status", "OFF");	//set initial status to offline
        //        m_contacts<<c;
        //qDebug()<<"Contact populated: "<<cut[1];
    }
    if (prefix == "RNG")
        acceptInvitation (cmd);
    if (prefix == "ILN")
    {
        //ILN trid statuscode account_name display_name clientid
        //find the contact and set its status
        MsnContact *c = findContact ("email", cut[3]);
        c->set ("status", cut[2]);
        c->set ("nickname", QUrl::fromPercentEncoding (cut[4].toUtf8 ()));
        c->set ("clientid", cut[5]);
        emit contactListChanged ();
    }
    if (prefix == "NLN")
    {
        //NLN status account_name display_name clientid
        MsnContact *c = findContact ("email", cut[2]);
        if (cut[1] != c->get ("status") || cut[3] != c->get ("nickname")
                || cut[4] != c->get ("clientid"))
        {
            c->set ("status", cut[1]);
            c->set ("nickname", cut[3]);
            c->set ("clientid", cut[4]);
            emit contactListChanged ();
        }
    }
    if (prefix == "FLN")
    {
        //FLN account_name
        MsnContact *c = findContact ("email", cut[1]);
        c->set ("status", "OFF");
        emit contactListChanged ();
    }
    if (prefix == "CHL")
        replyChallenge (cmd);
}

MsnContact *Msn::findContact (QString type, QString value)
{
    MsnContact *m;
    foreach (m, m_contacts)
    {
        if (m->get (type) == value)
            return m;
    }
    m = new MsnContact;
    m_contacts << m;
    return m;
}

    void
Msn::acceptInvitation (QString cmd)
{
    //check if there is an old session with the invitor, if there is, delete that session from m_sessions, and insert the new session in the exact position
    QRegExp rx ("RNG (\\S+) (\\S+) CKI (\\S+) (\\S+) (\\S+)");
    rx.indexIn (cmd);
    QString invitor = rx.cap (4);
    int pos = -1;
    int c = m_sessions.count ();
    QStringList oldHistory;
    for (int i = 0; i < c; i++)
        if (m_sessions[i]->talkingTo (invitor))
        {
            oldHistory = m_sessions[i]->m_history;
            m_sessions.removeAt (i);
            pos = i;
        }
    SwitchBoardSession *newSB =
        new SwitchBoardSession (oldHistory, m_username, cmd);
    if (pos == -1)
        m_sessions.append (newSB);
    else
        m_sessions.insert (pos, newSB);
    connect (newSB, SIGNAL (userTyping (QString)), this,
            SIGNAL (userTyping (QString)));
    connect (newSB, SIGNAL (sessionTyping (SwitchBoardSession *)), this,
            SIGNAL (sessionTyping (SwitchBoardSession *)));
    connect (newSB,
            SIGNAL (joinConversation (SwitchBoardSession *, QString, QString)),
            this,
            SLOT (userJoinLeftConversation
                (SwitchBoardSession *, QString, QString)));
    connect (newSB,
            SIGNAL (leftConversation (SwitchBoardSession *, QString, QString)),
            this,
            SLOT (userJoinLeftConversation
                (SwitchBoardSession *, QString, QString)));
    connect (newSB, SIGNAL (closed (SwitchBoardSession *)), this,
            SLOT (removeSB (SwitchBoardSession *)));
    connect (newSB,
            SIGNAL (msgReceived (QString, QString, SwitchBoardSession *)),
            this,
            SIGNAL (msgReceived (QString, QString, SwitchBoardSession *)));
    connect (newSB,
            SIGNAL (rawMsgReceived (QString, QString, SwitchBoardSession *)),
            this,
            SIGNAL (rawMsgReceived (QString, QString, SwitchBoardSession *)));
    emit syncSession ();
}

    SwitchBoardSession *
Msn::requestSBSession (QString receiver)
{
    //check if there is an existing session with this user.
    SwitchBoardSession *s;
    foreach (s, m_sessions) if (s->talkingTo (receiver))	//if there is, return it
    {
        s->reConnect ();
        return s;
    }
    //if there is not, request a new switch board session
    //qDebug()<<"creating new sb";
    SwitchBoardSession *newSB = new SwitchBoardSession (loginSession,
            (QStringList () <<
             m_username <<
             receiver));
    m_sessions.append (newSB);
    connect (newSB, SIGNAL (userTyping (QString)), this,
            SIGNAL (userTyping (QString)));
    connect (newSB,
            SIGNAL (joinConversation (SwitchBoardSession *, QString, QString)),
            this,
            SLOT (userJoinLeftConversation
                (SwitchBoardSession *, QString, QString)));
    connect (newSB,
            SIGNAL (leftConversation (SwitchBoardSession *, QString, QString)),
            this,
            SLOT (userJoinLeftConversation
                (SwitchBoardSession *, QString, QString)));
    connect (newSB, SIGNAL (closed (SwitchBoardSession *)), this,
            SLOT (removeSB (SwitchBoardSession *)));
    connect (newSB,
            SIGNAL (msgReceived (QString, QString, SwitchBoardSession *)),
            this,
            SIGNAL (msgReceived (QString, QString, SwitchBoardSession *)));
    connect (newSB,
            SIGNAL (rawMsgReceived (QString, QString, SwitchBoardSession *)),
            this,
            SIGNAL (rawMsgReceived (QString, QString, SwitchBoardSession *)));
    emit syncSession ();
    return newSB;
}

    void
Msn::userJoinLeftConversation (SwitchBoardSession * sb, QString email,
        QString reason)
{
    //qDebug()<<"The following user: "<<email<<" has "<<reason;
    if (reason == "join")
        emit userJoinSession (sb, email);
    if (reason == "timeout" || reason == "closed")
        emit userLeftSession (sb, email);
    emit syncSession ();
}

    void
Msn::removeSB (SwitchBoardSession * session)
{
    //search the list and remove this session from it
    int pos = m_sessions.indexOf (session);
    if (pos != -1)
        m_sessions.removeAt (pos);
    //qDebug()<<"session removed from pool";
    delete session;
    emit syncSession ();
}

    void
Msn::sendMsg (QString receiver, QString message)
{
    SwitchBoardSession *s = requestSBSession (receiver);
    if (s != NULL)
        s->sendMessage (receiver, message);
}

void
Msn::addContact (QString contact, QString group = "0")
{
    unblockContact (contact);
    loginSession->sendAndWait ("ADD %d FL %s %s %s\r\n", contact, contact, group);
}

    void
Msn::removeContact (QString contact)
{
    loginSession->sendAndWait ("REM %d AL %s\r\n", contact);
    loginSession->sendAndWait ("REM %d FL %s\r\n", contact);
}

    void
Msn::blockContact (QString contact)
{
    loginSession->sendAndWait ("REM %d AL %s\r\n", contact);
    loginSession->sendAndWait ("ADD %d BL %s %s\r\n", contact, contact);
}

    void
Msn::unblockContact (QString contact)
{
    loginSession->sendAndWait ("REM %d BL %s \r\n", contact);
    loginSession->sendAndWait ("ADD %d AL %s %s\r\n", contact, contact);
}

QList < MsnContact * >Msn::getContacts (QString status)
{
    //status={Online, Offline, Mobile, <All groups>}
    QList < MsnContact * >s;
    MsnContact *
        c;
    foreach (c, m_contacts)
    {
        if (c->get ("status") == status || c->get ("group") == status)	//TODO: filter by status
        {
            s << c;
        }
    }
    return s;
}

QList < QString > Msn::getContactsEmails (QString status)
{
    //status={Online, Offline, Mobile, <All groups>}
    QList < QString > s;
    QStringList
        statuses = status.split (",");
    MsnContact *
        c;
    //qDebug()<<"Total contacts: "<<m_contacts.count();
    foreach (QString st, statuses)
    {
        foreach (c, m_contacts)
        {
            if (c->get ("status") == st || c->get ("group") == st)	//TODO: filter by status
            {
                s << c->get ("email");
            }
        }
    }
    return s;
}
