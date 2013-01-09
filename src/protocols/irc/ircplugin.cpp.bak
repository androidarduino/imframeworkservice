#include "irc.h"

IRCPlugin::IRCPlugin()
{
//initialize an irc client
	d_client=new IRCClient("", 0);
	connect(d_client, SIGNAL(message(QString, QString, QString, QString)), this, SLOT(gotMsg(QString, QString, QString, QString))); 
}
IRCPlugin::~IRCPlugin()
{
//destroy the irc client
	d_client->disconnect();
}
bool IRCPlugin::available()
{
//return whether the client is available
	return d_client->d_connected;
}
QString& IRCPlugin::operator[](QString ref)
{
//return parameters of the client
	ref=ref.toLower();
	if(ref=="username")
		return d_client->d_userName;
	if(ref=="status")
		return d_client->d_status;
	if(ref=="realname")
		return d_client->d_realName;
	return *(new QString(""));
}
void IRCPlugin::sendMsg(Msg& msg)
{
//send message using options in msg
	QString user=msg["user"];
	QString message=msg["body"];	
	d_client->send(user, message);
}
void IRCPlugin::login()
{
//start login
	d_client->login();
}
QList<Msg> IRCPlugin::onlineBuddies()
{
//return online users, including channels and online users
	QList<Msg>& ret=*(new QList<Msg>());
	foreach(QString c, d_client->channels())
	{
		Msg *m=new Msg();
		(*m)["name"]=c;
		(*m)["uri"]=c+"_channel@"+d_client->d_server;
		(*m)["type"]="group";
		ret<<*m;
	}
	foreach(QString c, d_client->users())
	{
		Msg *m=new Msg();
		(*m)["name"]=c;
		(*m)["uri"]=c+"@"+d_client->d_server;
		(*m)["type"]="user";
		ret<<*m;
	}
	return ret;
}
void IRCPlugin::gotMsg(QString from, QString fromUri, QString receiver, QString msg)
{
//called when a message is received, emit signal
	Msg& m=*(new Msg());
	m["from"]=from;
	m["fromUri"]=fromUri;
	m["receiver"]=receiver;
	m["msg"]=msg;
	emit(msgArrived(m));
}
