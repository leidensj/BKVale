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

#endif // PRINTUTILS_H
