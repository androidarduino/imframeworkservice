#include "server.h"

IMService::IMService()
{
    connect(d_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    d_server.listen("IMFService");
}

IMService::~IMService()
{
    d_server.close();
}

void IMService::newConnection()
{
    IMClient* nc=nextPendingConnection();
    connect(nc, SIGNAL(readyRead()), this, SLOT(clientMsg()));
    clients<<nc;
}

bool IMService::startProtocol(QString& name)
{
    //load the protocol plugin and start it
}

bool IMService::stopProtocol(QString& name)
{
    //unload the plugin
}

void IMService::protocolMsg()
{
    //read the msg
    //find the client to send to
    //send to the client
}

void IMService::clientMsg()
{
    //read the msg
    IMClient* s=sender();
    if(!s->canReadLine())
        return;
    Msg msg(s->readLine());
    //find the protocol and send
    if(msg["protocol"]=="IMFServer")//requests to server
    {
        processClientRequest(s, msg);
    }
    if(msg["protocol"]!="")
    {
        foreach(IMProtocol* p, protocols)
            if(p->name==msg["protocol"])
                p->send(msg);
    }
    else
    {
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
    if(msg["command"]=="registerClient")
        registerClient(msg["clientName"], client);
    //TODO: more commands
}

void IMServerManager::registerClient(QString name, IMClient* client)
{
    client->name=name;
}
