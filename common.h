#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QRegularExpression>
#include <QDateTime>

namespace Functionality
{
  enum class Idx : int
  {
    User,
    Category,
    Product,
    Image,
    Employee,
    Supplier,
    Store,
    Purchase,
    Calculator,
    Reminder,
    ShoppingList,
    Shop,
    TimeCard,
    TimeCardControl,
    Settings,
    Report,
    Coupon,
    CouponRedemption,
    ProductCode,
    Login,
    Address,
    Phone,
    Inventory,
    Sector,
    Coin,
    _END,
  };

  QString idxToTableName(Idx idx);
  Idx tableNameToIdx(const QString& tableName);
}

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
  ASCII = 1 << 5,
  Alpha = 1 << 6
};

QString getRegEx(Input input);
}

namespace DateTime {
QDateTime server();
}
#endif // COMMON_H
