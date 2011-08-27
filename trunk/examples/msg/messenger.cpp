#include "messenger.h"

VRIM::VRIM()
{
    messenger=new IMFramework::Messenger("vrcats.com.chat");
    dest.setGeometry(QRect(0,0,400, 32));
    dest.setWindowTitle("Send Message To:");
    incoming.setGeometry(QRect(0,120,400, 600));
    incoming.setWindowTitle("Chat History");
    setGeometry(QRect(0,760,400, 32));
    setWindowTitle("Type message here, press enter to send");
    dest.show();
    incoming.show();
    connect(messenger, SIGNAL(get(QString, QString, long long)), this, SLOT(get(QString, QString, long long)));
}

void VRIM::get(QString from, QString msg, long long replyTo)
{
    Q_UNUSED(replyTo);
    incoming.append(QString("\r\n%1: %2").arg(from).arg(msg));
}

void VRIM::keyPressEvent(QKeyEvent* event)
{
    if(event->key()==16777220)//enter
    {
        QString destination=dest.currentText();
        QString message=text();
        QString msg=QString(":%1 %2").arg("").arg(message);
        messenger->send(msg, destination);
        qDebug()<<"sending message to: "<<destination<<": "<<msg;
        incoming.append(QString("\r\n%1: %2").arg("me").arg(msg));
        clear();
    }
    if(event->key()==16777268)//F5
    {
        dest.clear();
        dest.addItems(presence->allOnline());
    }
    QLineEdit::keyPressEvent(event);
}

