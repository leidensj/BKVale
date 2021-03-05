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

class Printer
{
public:
  bool print(const QString& msg, QString& error);

private:
  bool connect(QString& error);
  void disconnect();
  bool printByteArray(QByteArray& data, QString& error);
  bool printString(const QString& msg, QString& error);
  QSerialPort m_printerSerial;
  QTcpSocket m_printerTCP;
};

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
