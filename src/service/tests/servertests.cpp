#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QStringList>
#include "../server.h"

class TestMsg: public QObject
{
    Q_OBJECT

    private slots:
        void testOperatorSquareBrackets();
		void testConstructor_data();
        void testConstructor();
};

void TestMsg::testOperatorSquareBrackets()
{
}

void TestMsg::testConstructor_data()
{
	QTest::addColumn<QString>("message");
	QTest::addColumn<QString>("names");
	QTest::addColumn<QString>("values");

	QTest::newRow("msg t1")<<"<msg a=a1 b=b1>BODY</msg>"<<"a,b,body"<<"a1,b1,BODY";
	QTest::newRow("msg t2")<<"<msg x=c7>bodyc7</msg>"<<"x,body"<<"c7,bodyc7";
}

void TestMsg::testConstructor()
{
	QFETCH(QString, message);
	QFETCH(QString, names);
	QFETCH(QString, values);

	Msg msg(message.toUtf8());
	msg.print();

	QStringList namelist=names.split(",");
	QStringList valuelist=values.split(",");
	for(int i=0;i<namelist.count();i++)
		QVERIFY(msg[namelist[i]]==valuelist[i]);
}


class TestServer: public QObject
{
    Q_OBJECT

    private slots:
        void initTestCase();
        void testListening();
        void testRegistering();
        void cleanupTestCase();
};

void TestServer::initTestCase()
{

}

void TestServer::cleanupTestCase()
{

}

void TestServer::testListening()
{
}

void TestServer::testRegistering()
{
    //create a server object
    //create a client object
    //send a sample register message from client
    //verify that the client is named
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    TestMsg msg;
    TestServer server;
    QTest::qExec(&msg);
    QTest::qExec(&server);
    return 0;
}

#include "servertests.moc"
