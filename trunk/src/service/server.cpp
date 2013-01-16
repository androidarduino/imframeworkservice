#include "server.h"

IMService::IMService()
{
	connect(&d_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	d_server.listen("IMFService");
	qDebug()<<"listen successful";
	loadPlugins();
	qDebug()<<d_protocols.count();
}

IMService::~IMService()
{
	d_server.close();
	qDebug()<<"server closed";
}

IMClient::IMClient(QLocalSocket* socket)
{
	d_socket=socket;
	qDebug()<<"imclient created, socket bind";
}

void IMService::newConnection()
{
	qDebug()<<"new incoming connection";
	IMClient* nc=new IMClient(d_server.nextPendingConnection());
	connect(nc->d_socket, SIGNAL(readyRead()), this, SLOT(clientMsg()));
	d_clients<<nc;
	qDebug()<<nc->d_socket;
	qDebug()<<"new connection established";
}

bool IMService::controlProtocol(QString& , QString& )
{
	//TODO:
	//available commands:
	// enable, disable, etc.
	return true;
}

void IMService::protocolMsg(Msg& msg)
{
	QString target=msg["app"].toString();
	qDebug()<<"protocol msg received";
	//find the client to send to
	foreach(IMClient* c, d_clients)
	{
		if(c->name==target)
			c->d_socket->write(msg.d_string.toUtf8());
	}
}

void IMService::clientMsg()
{
	//read the msg
	qDebug()<<"got message from client: ";
	QLocalSocket* s=(QLocalSocket*)sender();
	qDebug()<<s->canReadLine();
	if(!s->canReadLine())
		return;
	Msg msg(s->readLine());
	qDebug()<<"got a client message";
	msg.print();
	IMClient* senderClient=0;
	foreach(IMClient* c, d_clients)
		if(c->d_socket==s)
			senderClient=c;
	if(senderClient==0)
		qDebug()<<"serious error occured";
	//find the protocol and send
	if(msg["protocol"]=="IMFServer")//requests to server
	{
		processClientRequest(senderClient, msg);
	}
	//TODO: find all protocols with the receiver online
	//if none, choose an offline protocol to send the message
	if(msg["protocol"]!="")
	{
		//if more than one, choose the one with desired protocol
		qDebug()<<"sending message on behalf of client";
	}
	else
	{
		//randomly choose a protocol to send, or select the highest ranked protocol
		qDebug()<<"sending message on behalf of client";
	}
}

IMServerManager::IMServerManager()
{
	qDebug()<<"server manager created";
}

IMServerManager::~IMServerManager()
{
	qDebug()<<"server manager destructed";

}

void IMServerManager::processClientRequest(IMClient* client, Msg& msg)
{
	//sample register message:
	//<msg protocol=IMFServer command=registerClient clientName=chessgame>please register me as chess client</msg>
	qDebug()<<"got client request";
	msg.print();
	if(msg["command"]=="registerClient")
		registerClient(msg["clientName"].toString(), client);
	//TODO: more commands
}

void IMServerManager::registerClient(QString name, IMClient* client)
{
	client->name=name;
	qDebug()<<"client "<<name<<" registered by manager";
}

bool IMServerManager::available()
{
	return true;
}
QList<Msg> IMServerManager::onlineBuddies()
{
	//TODO: collect online buddies from all plugins and return
	return QList<Msg>();
}
QString& IMServerManager::operator [](QString )
{
	QString& ret=*(new QString());
	return ret;
}
void IMServerManager::sendMsg(Msg& )
{
}

void IMServerManager::login()
{
}

bool IMService::loadPlugins()
{

/* test for irc protocol
   QPluginLoader pluginLoader("protocolplugins/libirc.so");
   pluginLoader.load();
   QObject *plugin = pluginLoader.instance();
   if (plugin) {
   IMProtocol* p=qobject_cast<IMProtocol*>(plugin);
   d_protocols << p;
   Msg config;
   config["user"]="vrcats@gmail.com";
   config["pass"]="";
   config["host"]="irc.freenode.net";
   config["port"]="6665";
   config["nickname"]="vrcats";
   p->init(config);
   p->login();
   }
 */
QPluginLoader pluginLoader("protocolplugins/libjabber.so");
qDebug()<<pluginLoader.load();
QObject *plugin = pluginLoader.instance();
qDebug()<<pluginLoader.errorString();
if (plugin) {
	IMProtocol* p=qobject_cast<IMProtocol*>(plugin);
	d_protocols << p;
	Msg config;
	config["user"]="imftester@jabber.org";
	config["pass"]="12345678";
	config["host"]="jabber.org";
	config["port"]="5222";
	config["domain"]="jabber.org";
	p->init(config);
	p->login();
}
return true;
}
