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
	QString target=msg["app"];
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
    if(msg["protocol"]!="")
    {
/*        foreach(IMProtocol* p, protocols)
            if(p->d_name==msg["protocol"])
                p->sendMsg(msg);
*/
        qDebug()<<"sending message on behalf of client";
    }
    else
    {
        qDebug()<<"sending message on behalf of client";
        //TODO: select the most suitable protocol to send mssg
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
        registerClient(msg["clientName"], client);
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
	/*
	//TODO: now it is a file driven plugin loading. We need to load the plugins driven by the config file.
     QDir pluginsDir(QCoreApplication::applicationDirPath());
#if defined(Q_OS_WIN)
     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
         pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
     if (pluginsDir.dirName() == "MacOS") {
         pluginsDir.cdUp();
         pluginsDir.cdUp();
         pluginsDir.cdUp();
     }
#endif
     pluginsDir.cd("protocolplugins");
     foreach (QString fileName, pluginsDir.entryList(QDir::Files)) 
     {
	 qDebug()<< "Found plugin: "<<fileName;
         QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
//qDebug()<<pluginLoader.load();
//qDebug()<<pluginLoader.isLoaded();
         QObject *plugin = pluginLoader.instance();
         if (plugin) {
			 IMProtocol* p=qobject_cast<IMProtocol*>(plugin);
             d_protocols << p;
			//TODO: call p->init("configurations");
             qDebug()<<"protocol plugin loaded for "<<fileName;
         }
     }
     qDebug()<<"protocol plugin not found";
     return false;
	 */
	
}
