#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QSerialPort>
#include <QDate>
#include <promissorynotewidget.h>

namespace PrintUtils
{
  bool print(QSerialPort& printer,
             const QString& msg,
             QString& error);

  bool initPrinter(QSerialPort& printer,
                   QString& error);

  QString buildHeader(const QDate& date);

  QString buildBody(const PromissoryNoteWidget& note);

  QString buildFooter(const QString& total);
}
#endif // PRINTUTILS_H
