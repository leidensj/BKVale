#ifndef PRINTER_H
#define PRINTER_H

#include <QtSerialPort>
#include <QTcpSocket>
#include <QByteArray>
#include "items/purchase.h"
#include "items/reminder.h"
#include "items/shoppinglist.h"
#include "items/coupon.h"

class Printer
{
public:
  bool print(const QString& msg, QString& error);
  bool print(const Purchase& o, QString& error);
  bool print(const Reminder& o, QString& error);
  bool print(const ShoppingList& o, bool bPrintCount, QString& error);
  bool print(const Coupon& o, QString& error);
  static QString st_strFullCut();

private:
  bool connect(QString& error);
  void disconnect();
  bool printByteArray(QByteArray& data, QString& error);
  bool printString(const QString& msg, QString& error);
  static QSerialPort m_printerSerial;
  static QTcpSocket m_printerTCP;
};

#endif // PRINTER_H
