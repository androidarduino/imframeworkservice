#include <QHttp>
#include <QDebug>
#include "printer.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QHttp http;
    printer p;
    QObject::connect(&http, SIGNAL(readyRead(const QHttpResponseHeader)), &p, SLOT(readyRead(const QHttpResponseHeader)));
    http.setHost("www.qtsoftware.com");
    http.get("/");
    return app.exec();
}
