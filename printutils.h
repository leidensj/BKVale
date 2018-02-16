#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QtSerialPort>
#include <QTcpSocket>
#include <QByteArray>
#include "note.h"
#include "reminder.h"
#include "item.h"
#include "consumption.h"
#include "settings.h"

namespace Printer
{

  bool printByteArray(QIODevice* printer,
                      QByteArray& data,
                      QString& error);

  bool printString(QIODevice* printer,
                   InterfaceType type,
                   const QString& msg,
                   QString& error);

  QString strCmdInit();
  QString strCmdFullCut();

  QByteArray imageToPrintVersion(const QByteArray& arImage);
}

namespace NotePrinter
{
  QString build(const Note& note,
                int number,
                const QString& user);
}

namespace ReminderPrinter
{
  QString build(const Reminder& r);
}

namespace ConsumptionPrinter
{
  QString build(qint64 date,
                const QVector<Consumption>& consumptions,
                const QVector<Item>& items,
                double total);

  QString build(const QVector<qint64>& dates,
                const QVector<double>& totals,
                double total);
}
#endif // PRINTUTILS_H
