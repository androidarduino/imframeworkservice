#ifndef MSNAUTH_H
#define MSNAUTH_H
#include <QHttp>
#include <unistd.h>
#include <QFile>

class MsnAuth:public QObject
{
Q_OBJECT
    public:
        MsnAuth(QString userName, QString password, QString ticket);
        void authenticate();
    private slots:
        void reply(int id, bool error);
    signals:
        void authCompleted (QString authenticationString);
    private:
        QHttp https;
        QString m_ticket, m_username, m_password, m_authString;
};

#endif
