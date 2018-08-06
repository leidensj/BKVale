#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QtSerialPort>
#include <QTcpSocket>
#include <QByteArray>
#include "note.h"
#include "reminder.h"
#include "product.h"
#include "person.h"
#include "shoppinglist.h"
#include "reservation.h"

namespace Printer
{

  bool printByteArray(QIODevice* printer,
                      QByteArray& data,
                      QString& error);

  bool printString(QIODevice* printer,
                   bool bIsEthernet,
                   const QString& msg,
                   QString& error);

  QString strCmdInit();
  QString strCmdFullCut();

  QByteArray imageToPrintVersion(const QByteArray& arImage);
}

namespace NotePrinter
{
  QString build(const Note& note,
                const QString& user);
}

namespace ReminderPrinter
{
  QString build(const Reminder& r);
}

namespace ShoppingListPrinter
{
  QString build(const ShoppingList& lst);
}

namespace ReservationPrinter
{
  QString build(const Reservation& lst);
}

#endif // PRINTUTILS_H
