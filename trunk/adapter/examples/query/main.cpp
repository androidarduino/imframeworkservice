#include "exam1.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QuerySender sender;
    QueryServer server;
    sender.linkTo("myself");//change this to my email address
    sender.query("select * from buddies");
    im.run();
    return app.exec();
}
