#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QtSerialPort>
#include <QTcpSocket>
#include <QByteArray>
#include "note.h"
#include "reminder.h"
#include "product.h"
#include "form.h"
#include "shoppinglist.h"
#include "reservation.h"
#include "discount.h"

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
  QString build(const ShoppingList& lst, bool bPrintCount);
}

namespace ReservationPrinter
{
  QString build(const Reservation& lst);
}

namespace DiscountPrinter
{
  QString build(const Discount& o);
  QString buildRedeem(const Discount& o);
}

#endif // PRINTUTILS_H
