#include "messenger.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
//    VRIM im;
//    im.run();
//    IMFramework::Messenger("test");
    VRIM im;
    im.show();
    return app.exec();
}
