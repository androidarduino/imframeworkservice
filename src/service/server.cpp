#include "server.h"

IMService::IMService()
{
    connect(&d_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    d_server.listen("IMFService");
}

IMService::~IMService()
{
    d_server.close();
}

IMClient::IMClient(QLocalSocket* socket)
{
    d_socket=socket;
}

void IMService::newConnection()
{
    qDebug()<<"new incoming connection";
    IMClient* nc=new IMClient(d_server.nextPendingConnection());
    connect(nc->d_socket, SIGNAL(readyRead()), this, SLOT(clientMsg()));
    d_clients<<nc;
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
    IMClient* s=(IMClient*)sender();
    if(!s->d_socket->canReadLine())
        return;
    Msg msg(s->d_socket->readLine());
    qDebug()<<"got a client message";
    msg.print();
    //find the protocol and send
    if(msg["protocol"]=="IMFServer")//requests to server
    {
        processClientRequest(s, msg);
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

}

IMServerManager::~IMServerManager()
{

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
}

bool IMServerManager::available()
{
    return true;
}
QList<Msg> IMServerManager::onlineBuddies()
{
    return QList<Msg>();
}
QString& IMServerManager::operator [](QString propertyName)
{
    QString& ret=*(new QString());
    return ret;
}
void IMServerManager::sendMsg(Msg& msg)
{
}

void IMServerManager::login()
{
}
