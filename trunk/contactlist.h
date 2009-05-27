#ifndef MSN_CONTACT_LIST
#define MSN_CONTACT_LIST
#include <QHttp>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QObject>
#include <QUrl>

class MsnContactList: public QObject
{
    Q_OBJECT
    public:
        MsnContactList(QString mspAuth);
        bool requestMembershipList();
        bool requestAddressBook();
    private:
        QHttp http1, http2;
        QHttpRequestHeader header1, header2;
        QString loadSoapRequestFile(QString fileName);
        QString m_mspAuth;
    private slots:
        void gotMembershipList(int id, bool error);
        void gotAddressBook(int id, bool error);
        void dataArrived(bool error);
};

#endif
