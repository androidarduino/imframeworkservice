#include "msg.h"

Msg::Msg(QByteArray msg)
{
    //<msg a=a1 b=b1 c=c1>msgbody</msg>
    QRegExp rxMain("\\s*<\\s*msg\\s+([^>]*)>([^<]*)<\\s*/msg\\s*>\\s*");
    QRegExp rxParameters("(\\w*)=(\\w*)");
    d_string=QString(msg);
    if(rxMain.indexIn(d_string)==-1)
	{
		qDebug()<<"no message found";
        return;
	}
    QString body=rxMain.cap(2);
	items["body"]=body;
    QString parameters=rxMain.cap(1);
	qDebug()<<"parameters: "<<parameters;
    int pos=0;
    while((pos=rxParameters.indexIn(parameters, pos))!=-1)
    {
        QString name=rxParameters.cap(1);
        QString value=rxParameters.cap(2);
        items[name]=value;
        pos += rxParameters.matchedLength();
		//qDebug()<<"found a parameter:"<<pos<<name<<value<<rxParameters.matchedLength()<<parameters;
    }
}

void Msg::print()
{
	qDebug()<<"MSG: "<<items;
}

QString& Msg::operator [](QString name)
{
    return items[name];
}

Msg::Msg(QString json)
{

}

void Msg::fromJson(QString json)
{

}

QString Msg::toJson()
{
	QString ret="{\n";
	foreach(QString key, items.keys())
		ret+="\t"+key+":\""+items.value(key)+"\";\n";
	ret+="}\n";
	return ret;
}
