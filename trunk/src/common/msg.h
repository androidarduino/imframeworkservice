#ifndef MSG_H
#define MSG_H

#include <QString>
#include <QByteArray>
#include <QRegExp>
#include <QDebug>
#include <QVariantMap>
#include <qjson/parser.h>
#include <qjson/serializer.h>

/*
   Abstract of messages transfering between server and client/protocol, convert a message string to data structure, and back
 */

class Msg
{
	/*
	   service to client message protocol:
	   "<msg protocol=irc1 app=messagingclient threadId=1>this is a message</msg>"
	   "<msg protocol=IMServer app=registerService name=messagingclient status=online></msg>
	 */
	public:
		Msg ();
		Msg (QString json);
		Msg (const Msg & msg);
		virtual ~ Msg ();
		void fromJson (QString json);
		QVariant & operator [] (QString name);
		QString toJson () const;
		void print ();
		QString d_string;
		QVariantMap& toMap(){return items;}
		void insert(QString, Msg&);
	private:
		QVariantMap items;
};

Q_DECLARE_METATYPE (Msg)
#endif
