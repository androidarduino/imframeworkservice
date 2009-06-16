#ifndef IMCHANNEL
#define IMCHANNEL
#include <QString>
#include <QMap>
#include <QDebug>
#include "msn.h"

class IMChannel:public QObject
{
Q_OBJECT
    public:
    IMChannel ();		//constructor
  void initChannel (QString commandFormat = defaultCommandFormat);	//init by inviting others
private:
    QStringList chooseContacts ();	//popup a dialog to choose who to involve in the channel conversation
  private slots:void receivedMessage (QString message, QString sender,
				      SwitchBoardSession * session);
  void statusChanged ();
protected:
    public slots:int sendCommand (QString command);	//send a command to all other participants in the channel
  int sendCommand (QString receiver, QString command);	//send a command to a single receiver in the channel
    signals:
  //void msg (int index, QString command, QMap < QString > parameters, QString sender);
  //when someone in the channel sends a command, trigger this signal
  void statusChanged (QString participant, QString status);	//when someone's status has changed, trigger this signal
  void error (QString errorType, QString errorMessage);	//when there is any error, eg. network fault, trigger this signal
  void ready (bool approved);	//when the initialization is completed, and ready to send/receive commands
};

#endif
