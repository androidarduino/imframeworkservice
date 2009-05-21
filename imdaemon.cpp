#include "imdaemon.h"
#include <QtopiaApplication>
#include <QDebug>
#include <QCopChannel>

IMDaemon::IMDaemon (QWidget *, Qt::WFlags &)
{
  settingsDlg.setupUi (this);
  //show wait screen
  waitScreen = new QWaitWidget (this);
  //do i need to setup a ui here?
  msn = new Msn ();
  //keep it running even hidden
  QtopiaApplication::instance ()->registerRunningTask ("imdaemon");
  //connect msn signals to this
  connect (msn,
	   SIGNAL (rawMsgReceived (QString, QString, SwitchBoardSession *)),
	   this, SLOT (msgReceived (QString, QString, SwitchBoardSession *)));
  connect (msn, SIGNAL (loginFinish ()), this, SLOT (readyToGo ()));
  connect (settingsDlg.login, SIGNAL (clicked ()), this, SLOT (login ()));
  //connect slots to IPC pool
  QtopiaIpcAdaptor *adaptor = new QtopiaIpcAdaptor ("QPE/IMFramework");
  QtopiaIpcAdaptor::connect (adaptor, MESSAGE (invite (QString, QString)),
			     this, SLOT (invite (QString, QString)));
  QtopiaIpcAdaptor::connect (adaptor,
			     MESSAGE (sendMessage (QString, QString)), this,
			     SLOT (sendMessage (QString, QString)));
  QtopiaIpcAdaptor::connect (this, SIGNAL (gotMessage (QString, QString)),
			     adaptor,
			     MESSAGE (receiveMessage (QString, QString)));
  QtopiaIpcAdaptor::connect (this, SIGNAL (ready (QString)), adaptor,
			     MESSAGE (ready (QString)));
  //load icon resource
}

void
IMDaemon::login ()
{
  waitScreen->show ();
  msn->setUsernamePassword (settingsDlg.username->text (),
			    settingsDlg.password->text ());
  myEmail = settingsDlg.username->text ();
  if(!msn->loginMsn ())
      alert("Can not connect to Server, please check your internet connection.");
}

bool IMDaemon::alert(QString msg)
{
	if(QMessageBox::Yes==QMessageBox::question(this, "IM Framework",msg))
		return true;
	else
		return false;
}

void
IMDaemon::readyToGo ()
{
  waitScreen->hide ();
  settingsDlg.status->setText ("Online");
  settingsDlg.statusIcon->setPixmap (*(new QPixmap (":/pics/online.png")));
}

IMDaemon::~IMDaemon ()
{
}

void
IMDaemon::sendMessage (QString receiver, QString message)
{
  if (receiver == "getParticipants()")
    {
      emit gotMessage ("syncParticipant()", partiesBank[message]);
      partiesBank[message] = "";
      return;
    }
  if (receiver == myEmail)
    return;
  msn->sendMsg (receiver, message);	//send message
}

void
IMDaemon::msgReceived (QString message, QString sender, SwitchBoardSession *)
{
  qDebug () << "IMDaemon got a message from" << sender << ":" << message;
  //filter out my own messages which are from "me"
  if (sender == "me")
    return;
  //check whether it is a control message, if so, launch application accordingly
  QStringList sp = message.split (":");
  if (sp[0] == "imframework")
    {
      QString program = sp[1].toLower ();
      QString participants = sp[2];
      QString attachedMessage = sp[3];
      qDebug () << ".......................launching: " << program <<
	participants << attachedMessage;
      //now launch the application
      QtopiaIpcAdaptor *adaptor =
	new QtopiaIpcAdaptor ("QPE/Application/" + program);
      adaptor->send (MESSAGE (raise ()));
      partiesBank[program] = participants;
      return;
    }
  emit gotMessage (sender, message);
}

void
IMDaemon::invite (QString program, QString attachedMessage)
{
  SelectBuddyDialog dlg (this, 0);
  dlg.populateContacts (msn->getContactsEmails ("NLN,AWY,IDL,BSY"));
  qDebug () << msn->getContactsEmails ("NLN,AWY,IDL,BSY");
  if (dlg.exec () == QDialog::Rejected)
    {
      emit ready ("Cancelled");
      return;
    }
  QStringList ret = dlg.selectedBuddies ();
  qDebug () << "emitting ready() signal";
  foreach (QString s, ret)
    sendMessage (s,
		 "imframework:" + program + ":" + ret.join (",") + "," +
		 myEmail + ":" + attachedMessage);
  emit ready (ret.join (","));
}
