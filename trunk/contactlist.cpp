#include "contactlist.h"

MsnContactList::MsnContactList(QString mspAuth)
{
    m_mspAuth=mspAuth;
}

bool MsnContactList::requestMembershipList()
{
    QString soap=""+loadSoapRequestFile("membershiplist.xml");
    http1.setHost("contacts.msn.com");
    header1.setRequest("POST","/abservice/SharingService.asmx");
    header1.setContentType("text/xml; charset=utf-8");
    header1.addValue("SOAPAction","http://www.msn.com/webservices/AddressBook/FindMembership");
    header1.addValue("Cookie", QString("MSPAuth=%1").arg(m_mspAuth));
    header1.setValue("Host", "contacts.msn.com");
    header1.setValue("Content-Length", QString("%1").arg(soap.toUtf8().length()));
    connect(&http1, SIGNAL(done(bool)), this, SLOT(dataArrived(bool)));
    http1.request(header1, soap.toUtf8());
    qDebug()<<"started get"<<header1.toString();
    return true;
}

void MsnContactList::dataArrived(bool)// error)
{
    //qDebug()<<"data arrived: "<<http1.error()<<http1.readAll()<<http1.lastResponse().toString();
}

bool MsnContactList::requestAddressBook()
{
    QString soap=loadSoapRequestFile("addressbook.xml");
    http2.setHost("contacts.msn.com");
    header2.setRequest("POST","/abservice/abservice.asmx");
    header2.setContentType("text/xml; charset=utf-8");
    header2.addValue("SOAPAction","http://www.msn.com/webservices/AddressBook/ABFindAll");
    header2.addValue("Cookie", QString("MSPAuth=%1").arg(m_mspAuth));
    header2.setValue("Host", "contacts.msn.com");
    header2.setValue("Content-Length", QString("%1").arg(soap.toUtf8().length()));
    connect(&http2, SIGNAL(requestFinished(int, bool)), this, SLOT(gotAddressBook(int, bool)));
    http2.request(header2, soap.toUtf8());
    return true;
}

QString MsnContactList::loadSoapRequestFile(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data=file.readAll();
    file.close();
    return data;
}

void MsnContactList::gotMembershipList(int id, bool error)
{
    qDebug()<<"got membership list: "<<id<<error<<http1.errorString()<<http1.error();
    qDebug()<<http1.lastResponse().toString();
    qDebug()<<http1.readAll();
}

void MsnContactList::gotAddressBook(int id, bool error)
{
    qDebug()<<"got addressbook: "<<id<<error<<http2.errorString();
    QString content=http2.readAll();
    if(content=="")
        return;
    parse(content);
    emit gotBuddyList();
}

QString MsnContactList::ml()
{
    QString t="<ml l=\"1\">";
    QString prevDomain="";
    foreach(QString e, buddies.keys())
    {
        QStringList l=e.split("@");
        QString domain=l[1];
        QString contact=l[0];
        //add domain, if the same, ignore
        if(domain!=prevDomain)
        {
            if(prevDomain!="")
                t+="</d>";
            t+=QString("<d n=\"%1\">").arg(domain);
        }
        //add contact name
        t+=QString("<c n=\"%1\" l=\"3\" t=\"1\" />").arg(contact);
        //if not the same, close domain
        prevDomain=domain;
    }
    t+="</d></ml>";
    qDebug()<<t;
    return t;
    //return "<ml l=\"1\"><d n=\"hotmail.com\"><c n=\"robot_liuzheng\" l=\"3\" t=\"1\" /></d></ml>";
}

void MsnContactList::parse(QString& content)
{
    buddies.clear();
    QDomDocument doc;
    doc.setContent(content);
    QDomNodeList list=doc.elementsByTagName("passportName");
    //TODO: temporarily solution, populate all information later
    for(int n=0;n<list.count();n++)
    {
        buddies[list.at(n).toElement().text()]="good";
        qDebug()<<list.at(n).toElement().text();
    }
}



