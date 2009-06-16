#include "imchannel.h"

IMChannel::IMChannel (QStringList participants, QString commandFormat)	//constructor
{
  //check if there is any msn instance, if there is, grab the instance, else, create one
  //check if the msn instance is logged in, if not, prompt login
  //wait for login to complete then continue
  //init the command format regular expression object
  //connect the receive signal to commandReceived
  //connect the statusChanged signal to statusChanged
  //connect other signals to error
  //connect syncSession to statusChanged
  //create a switchboard session, and invite all participants to this session
}

IMChannel::IMChannel (SwitchBoardSession * initSession)
{
  //init the channel by accepting the other party's invitation
  //set the initSession to member session
  //? how to get the existing participants in the session?
}
int
IMChannel::sendCommand (QString command)	//send a command to all other participants in the channel
{
  //call the other sendCommand to send command to all other participants
}
int
IMChannel::sendCommand (QString receiver, QString command)	//send a command to a specific receiver in the channel
{
  //check if is logged in, if not, prompt login
  //check if the receiver is in this switchboard session
  //if in, send command to the receiver
  //else, invite the receiver and send command to him
}
void
IMChannel::receivedMessage (QString message, QString sender,
			    SwitchBoardSession * session)
{
  //process the incoming message and issue commandReceived signal
}
void
IMChannel::statusChanged ()
{
  //check the new status of participants and do things accordingly
}
