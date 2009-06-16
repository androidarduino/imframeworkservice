#include <QHttp>
#include <QDebug>
class printer:public QObject
{
    Q_OBJECT
    public:
        printer()
        {
            
        }
    public slots:
        void readyRead(const QHttpResponseHeader & resp)
        {
            qDebug()<<resp.toString();
        }
};

