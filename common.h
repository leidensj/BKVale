#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QRegExp>
#include <QDateTime>

namespace Data
{
enum class Type
{
  Integer,
  Money,
  Ammount,
  Fmt,
  Percentage
};

QString strMoney(double value, bool bCurrency = true);
QString strAmmount(double value);
QString strFmt(double value);
QString strInt(double value);
QString strPercentage(double value);
bool areEqual(double v1, double v2, Type type);
QString str(double value, Type type);
}

namespace Text
{
enum class Input
{
  All = 1 << 0,
  AlphanumericAndSpaces = 1 << 1,
  Alphanumeric = 1 << 2,
  Numeric = 1 << 3,
  BasicMath = 1 << 4,
  ASCII = 1 << 5
};

QString getRegEx(Input input);
}

namespace DateTime {
QDateTime server();
}
#endif // COMMON_H
