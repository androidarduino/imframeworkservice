#include <QApplication>
#include "interface.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
//    IMService service;
//    service.start();
    IMFramework::Messenger messenger("test");
    return app.exec();
}
