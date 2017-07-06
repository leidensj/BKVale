#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QtSerialPort/QtSerialPort>
#include <note.h>

namespace Printer
{
  bool init(QSerialPort& printer,
            QString& error);

  bool print(QSerialPort& printer,
             const QString& msg,
             QString& error);
}

namespace NotePrinter
{
  QString build(const Note& note);
}

namespace ReminderPrinter
{
  QString build(const QString& title,
                const QString& msg,
                bool bExpandFont);
}
#endif // PRINTUTILS_H
