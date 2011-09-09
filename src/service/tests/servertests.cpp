#include <QtTest/QtTest>
#include <QApplication>
//#include "../server.h"

class TestMsg: public QObject
{
    Q_OBJECT

    private slots:
        void testOperatorSquareBrackets();
        void testConstructor();
};

void TestMsg::testOperatorSquareBrackets()
{
}

void TestMsg::testConstructor()
{
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
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    TestMsg msg;
    TestServer server;
    QTest::qExec(&msg);
    QTest::qExec(&server);
    return 0;
}

#include "servertests.moc"
