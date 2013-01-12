#include "ircplugin.h"

IMIRCPlugin::IMIRCPlugin()
{
}
void IMIRCPlugin::init(Msg& config)
{
//initialize an irc client
	//example parameters: "irc.freenode.net|6665|user0312|mypassword"
	d_host=config["host"].toString();
	d_port=config["port"].toString();
	d_user=config["user"].toString();
	d_pass=config["pass"].toString();
	d_client=new IRCClient(d_host, d_port.toInt());
	connect(d_client, SIGNAL(message(QString, QString, QString, QString)), this, SLOT(gotMsg(QString, QString, QString, QString))); 
}
IMIRCPlugin::~IMIRCPlugin()
{
//destroy the irc client
	d_client->disconnect();
}
bool IMIRCPlugin::available()
{
//return whether the client is available
	return d_client->d_connected;
}
QString& IMIRCPlugin::operator[](QString ref)
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
void IMIRCPlugin::sendMsg(Msg& msg)
{
//send message using options in msg
	QString user=msg["user"].toString();
	QString message=msg["body"].toString();	
	d_client->send(user, message);
}
void IMIRCPlugin::login()
{
    qDebug()<<"irc logging in...";
	d_client->login();
}
QList<Msg> IMIRCPlugin::onlineBuddies()
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

void IMIRCPlugin::gotMsg(QString from, QString fromUri, QString receiver, QString msg)
{
//called when a message is received, emit signal
	Msg& m=*(new Msg());
	m["from"]=from;
	m["fromUri"]=fromUri;
	m["receiver"]=receiver;
	m["msg"]=msg;
	emit(msgArrived(m));
}
Q_EXPORT_PLUGIN2(imircplugin, IMIRCPlugin);
