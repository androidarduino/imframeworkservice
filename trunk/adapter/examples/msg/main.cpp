#include "exam1.h"

VRIM::VRIM()
{
}

void VRIM::run()
{
    qDebug()<<"Messenger started. Available commands: /help /quit /list";
    qDebug()<<"     to send message, type :someone@somehost.com/vrcats.com.Exam1 test message for you";
    std::string s;
    while(1)
    {
        //scan for input
        std::getline(std::cin, s);
        QString cmd;
        cmd=cmd.fromStdString(s);
        //if it is a send command, send the message: ":vrcats@vrcats.com/vrcats.com.Exam1 test message for you"
        QRegExp rx(":([a-zA-Z0-9._%-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4})(?:[/]?([^ ]*)) (.*)");
        if(rx.indexOf(cmd)!=-1)
        {
            QString dest=rx.cap(1);
            QString app=rx.cap(2);
            QString msg=rx.cap(3);
            if(app=="")
                app="vrcat.com/Exam1";
            msg=QString(":%1 %2").arg(app).arg(msg);
            messenger.send(msg, dest);
        }
        //if it is a list command, list available
        cmd=cmd.toLower();
        if(cmd=="/list")
            qDebug()<<"Listing online buddies:";
        //if it is a quit command, quit
        if(cmd=="/quit")
            exit(0);
        //if it is a help command, display help
        if(cmd=="help"||cmd=="/help")
        {
            qDebug()<<"type /help to get help; type /list to list online users; type /quit to quit application";
            qDebug()<<"to send message to abc@d.com, type "":abc@d.com/vrcats.com.Exam1 This is a test message.""";
        }
    }
}


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    VRIM im;
    im.run();
    return app.exec();
}
