#ifndef MSNCONTACT_H
#define MSNCONTACT_H
#include <QString>
#include <QMap>
#include "switchboard.h"
#include <QDebug>

class MsnContact
{
public:
  MsnContact ()
  {
  };
  bool set (QString pName, QString pValue)
  {
    properties.insert (pName, pValue);
    return true;
  };
  QString get (QString pName)
  {
    return properties[pName];
  };
  bool delProperty (QString pName)
  {
    properties.remove (pName);
    return true;
  };
  void print ()
  {
    qDebug () << "MSN Contact:\n" << properties << "\n\n";
  }
  QString toString ()
  {
    return properties["nickname"] + "\r\n" + properties["group"] + "\r\n" +
      properties["status"] + "\r\n" + properties["email"];
  }
private:
  QMap < QString, QString > properties;
};
#endif
