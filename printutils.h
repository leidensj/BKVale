#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QtSerialPort>
#include <QTcpSocket>
#include <QByteArray>
#include "items/purchase.h"
#include "items/reminder.h"
#include "items/product.h"
#include "items/form.h"
#include "items/shoppinglist.h"
#include "items/discount.h"

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

namespace PurchasePrinter
{
  QString build(const Purchase& o);
}

namespace ReminderPrinter
{
  QString build(const Reminder& r);
}

namespace ShoppingListPrinter
{
  QString build(const ShoppingList& lst, bool bPrintCount);
}

namespace DiscountPrinter
{
  QString build(const Discount& o);
  QString buildRedeem(const Discount& o);
}

#endif // PRINTUTILS_H
