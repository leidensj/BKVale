#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QSerialPort>
#include <note.h>

namespace PrintUtils
{
  bool print(QSerialPort& printer,
             const QString& msg,
             QString& error);

  bool initPrinter(QSerialPort& printer,
                   QString& error);

  QString buildNote(const Note& note);
}
#endif // PRINTUTILS_H
