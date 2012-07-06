#include "client.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MyIM im;
    im.show();
    im.connectServer();
    //qDebug()<<im.d_socket.errorString();
    return app.exec();
}
