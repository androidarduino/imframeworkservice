#include "msg.h"
Msg::Msg ()
{
	Msg (QByteAray ());
}

Msg::~Msg ()
{

}

Msg::Msg (const Msg& msg)
{
	Msg (msg.toJson());
}

	void
Msg::print ()
{
	qDebug () << "MSG: " << items;
}

void Msg::insert(QString key, Msg& msg)
{
	items[key]=msg.toMap();
}

QVariant & Msg::operator [](QString name)
{
	return items[name];
}

Msg::Msg (QString msg)
{
	fromJson(msg);
}

void Msg::fromJson (QString json)
{
	QJson::Parser parser;
	bool ok;
	items=parser.parse(json.toUtf8(), &ok).toMap();
}

QString Msg::toJson () const
{
	QJson::Serializer serilizer;
	return serilizer.serialize(items);
}
