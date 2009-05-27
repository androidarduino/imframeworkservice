#ifndef MSNSESSION_H
#define MSNSESSION_H
#include <QTcpSocket>
#include <QString>
#include <QStringList>

class MsnSession:public QObject
{
Q_OBJECT public:
    QString mspAuth;
    MsnSession (QString server, int port);	//sometimes it is easier with this
    MsnSession (QString serverAndPort);	//parameter is something like "192.168.0.1:1683"
  int sendCommand (const char *cmd, QString p1 = "", QString p2 = "", QString p3 = "", QString p4 = "");	//send a command to server and return the index of the command
  QString sendAndWait (const char *cmd, QString p1 = "", QString p2 = "", QString p3 = "", QString p4 = "");	//send a command to server and wait for the answer, return answer
  QString getAnswer (int cmdIndex, int timeOut = 2000);	//wait for the answer to arrive, and grab it
  QString findReg (QString regExp, QString findIn, int whichText);
  int state ();
  private slots:void received ();
    signals:void incomingMsg (QString answer);
  void disconnected ();
private:
    QTcpSocket * m_socket;
  QStringList m_answers;
  QString sbServer;
  int sbPort;
  int m_index;
  bool init (QString server, int port);
};
#endif
