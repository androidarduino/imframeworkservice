#include <QApplication>
#include <QDebug>
#include "../msg.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	Msg msg, nextmsg;
	nextmsg["a"]="test";
	nextmsg["ba"]=123;
	nextmsg["ac"]=true;
	msg["user"]="abc@abc.com";
	msg["pass"]="mypassword";
	msg["host"]="jabber.com";
	msg["port"]="2345";
	msg["domain"]="abc.com";
	msg["err"].setValue(nextmsg.toMap());
	qDebug()<<msg.toJson();
	return app.exec();
}
