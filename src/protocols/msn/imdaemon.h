#ifndef IMDAEMON_H
#define IMDAEMON_H
#include "msn.h"
#include "ui_settings.h"
#include <QDialog>
#include <QList>
#include <QMap>

class IMDaemon:public QWidget
{
Q_OBJECT public:
    IMDaemon (QWidget * parent, Qt::WFlags & flags);
    virtual ~ IMDaemon ();
    signals:void gotMessage (QString sender, QString content);	//when a msg comes in
  void ready (QString participants);
private:
    Msn * msn;
    QMap < QString, QString > partiesBank;	//for caching the parties
  void launchApplication (QString applicationName);	//use qcop QPE/Application/tictactoe "raise()" to call
    Ui::IMSettings settingsDlg;
  QString myEmail;
  bool alert (QString msg);
  private slots:void msgReceived (QString message, QString sender,
				  SwitchBoardSession * session);
  void readyToGo ();
  void login ();
  public slots:void sendMessage (QString receiver, QString message);	//send a message
  void invite (QString program, QString attachedMessage);	//invite someone to use a program
};
#endif
