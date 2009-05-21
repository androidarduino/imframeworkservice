#ifndef MIME_H
#define MIME_H
#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QUrl>

class Mime
{
public:
  Mime (QString input)
  {
    m_data = input;
    parse ();
  }
  QString getHtml ()
  {
    //TODO: return html rich text with effects indicated in mime header
    return payload;
  }
  QString payload, mimeVersion, contentType, userAgent, xMmsImFormat,
    typingUser, font, effect, color, charset, pitchAndFamily;
  void print ()
  {
    QString s =
      QString
      ("Version: %1\nType: %2\nAgent: %3\nFormat: %4\nTyping: %5\nFont: %6\nEffect: %7\nColor: %8\nCharset: %9\nPF: %10\nPayload: %11\n").
      arg (mimeVersion).arg (contentType).arg (userAgent).arg (xMmsImFormat).
      arg (typingUser).arg (font).arg (effect).arg (color).arg (charset).
      arg (pitchAndFamily).arg (payload);
    qDebug () << s;
  }
private:
  QString m_data;
  void parse ()
  {
    QStringList lines = m_data.split ("\r\n");
    QString line, left, right;
    foreach (line, lines)
    {
      int p = line.indexOf (":");
      left = line.left (p).toLower ();
      right = line.mid (p + 2, -1);
      //qDebug()<<left<<right;
      if (left == "mime-version")
	mimeVersion = right;
      else if (left == "content-type")
	contentType = right;
      else if (left == "user-agent")
	userAgent = right;
      else if (left == "x-mms-im-format")
	xMmsImFormat = right;
      else if (left == "typinguser")
	typingUser = right;
      else
	payload = payload + line;
    }
    if (xMmsImFormat != "")
      {
	QRegExp rx1 ("FN=([^;]+)");
	rx1.indexIn (xMmsImFormat);
	font = QUrl::fromPercentEncoding (rx1.cap (1).toAscii ());
	QRegExp rx2 ("EF=([^;]+)");
	rx2.indexIn (xMmsImFormat);
	effect = rx2.cap (1);
	QRegExp rx3 ("CO=([^;]+)");
	rx3.indexIn (xMmsImFormat);
	color = rx3.cap (1);
	QRegExp rx4 ("CS=([^;]+)");
	rx4.indexIn (xMmsImFormat);
	charset = rx4.cap (1);
	QRegExp rx5 ("PF=([^;]+)");
	rx5.indexIn (xMmsImFormat);
	pitchAndFamily = rx5.cap (1);
      }
  }
};
#endif
