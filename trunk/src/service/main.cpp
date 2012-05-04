#include <QApplication>
#include "server.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    IMService service;
    return app.exec();
}
