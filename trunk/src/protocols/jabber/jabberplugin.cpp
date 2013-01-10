#include "jabberplugin.h"

IMJabberPlugin::IMJabberPlugin()
{
}
void IMJabberPlugin::init(QString parameters)
{
//initialize a jabber client
//example parameters: "talk.google.com|5552|test4re3@gmail.com|testpassword|gmail.com"
	QStringList p=parameters.split("|");
	d_host=p[0];
	d_port=p[1];
	d_user=p[2];
	d_pass=p[3];
	d_domain=p[4];
	d_client=new QXmppClient(this);
	connect(d_client, SIGNAL(messageReceived(const QXmppMessage&)), this, SLOT(messageReceived(const QXmppMessage&)));
}
IMJabberPlugin::~IMJabberPlugin()
{
//destroy the jabber client
	d_client->disconnectFromServer();
}
bool IMJabberPlugin::available()
{
//return whether the client is available
	return d_client->isConnected();
}
QString& IMJabberPlugin::operator[](QString ref)
{
//return parameters of the client
	ref=ref.toLower();
	if(ref=="username")
		return d_user;
	return *(new QString(""));
}
void IMJabberPlugin::sendMsg(Msg& msg)
{
//send message using options in msg
	QString user=msg["user"];
	QString message=msg["body"];
	QXmppMessage m(d_user, user, message);
	d_client->sendPacket(m);
}
void IMJabberPlugin::login()
{
    qDebug()<<"jabber logging in...";
	d_client->connectToServer(d_host, d_user, d_pass, d_domain, d_port.toInt());
}
QList<Msg> IMJabberPlugin::onlineBuddies()
{
//return online users, including channels and online users
//TODO: d_client->rosterManager()->getAllPresence();
//returns QMap<QString bareJID, QXmppPresence>
//loop and find QXmppPresence.type()==QXmppPresence::Available, collect them and return
}

void IMJabberPlugin::messageReceived(const QXmppMessage& msg)
{
	emit(gotMsg(msg.from(),  msg.from(), msg.to(), msg.body()));
}

void IMJabberPlugin::gotMsg(QString from, QString fromUri, QString receiver, QString msg)
{
//called when a message is received, emit signal
	Msg& m=*(new Msg());
	m["from"]=from;
	m["fromUri"]=fromUri;
	m["receiver"]=receiver;
	m["msg"]=msg;
	emit(msgArrived(m));
}
Q_EXPORT_PLUGIN2(imjabberplugin, IMJabberPlugin);
