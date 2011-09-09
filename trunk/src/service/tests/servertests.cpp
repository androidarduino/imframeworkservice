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
        void testListening();
        void testRegistering();
};

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
    return app.exec();
}

#include "servertests.moc"
