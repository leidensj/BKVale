#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <QSerialPort>
#include <QString>
#include <QDate>
#include <vector>

namespace PrintUtils
{
  bool print(QSerialPort& printer,
             const QString& msg,
             QString& error);

  bool initPrinter(QSerialPort& printer,
                   QString& error);

  void buildHeader(const QDate& date,
                   QString& header);

  void buildBody(const std::vector<std::vector<QString>>& tableContent,
                 QString& body);

  void buildFooter(const QString& total,
                   QString& footer);
}
#endif // PRINTUTILS_H
