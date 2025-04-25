#include "common.h"
#include "defines.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>


QString Functionality::idxToTableName(Idx idx)
{
  switch (idx)
  {
    case Idx::User:
      return USER_SQL_TABLE_NAME;
    case Idx::Category:
      return CATEGORY_SQL_TABLE_NAME;
    case Idx::Product:
      return PRODUCT_SQL_TABLE_NAME;
    case Idx::Image:
      return IMAGE_SQL_TABLE_NAME;
    case Idx::Employee:
      return EMPLOYEE_SQL_TABLE_NAME;
    case Idx::Supplier:
      return SUPPLIER_SQL_TABLE_NAME;
    case Idx::Store:
      return STORE_SQL_TABLE_NAME;
    case Idx::Purchase:
      return PURCHASE_SQL_TABLE_NAME;
    case Idx::Reminder:
      return REMINDER_SQL_TABLE_NAME;
    case Idx::ShoppingList:
      return SHOPPING_LIST_SQL_TABLE_NAME;
    case Idx::Coupon:
      return COUPON_SQL_TABLE_NAME;
    case Idx::Phone:
      return PHONE_SQL_TABLE_NAME;
    case Idx::Address:
      return ADDRESS_SQL_TABLE_NAME;
    case Idx::ProductCode:
      return PRODUCT_CODE_ITEMS_SQL_TABLE_NAME;
    case Idx::Inventory:
      return INVENTORY_SQL_TABLE_NAME;
    case Idx::Login:
      return LOGIN_SQL_TABLE_NAME;
    case Idx::Sector:
      return SECTOR_SQL_TABLE_NAME;
    case Idx::Coin:
      return COIN_SQL_TABLE_NAME;
    case Idx::Cash:
      return CASH_SQL_TABLE_NAME;
    case Idx::CashClosing:
      return CASH_CLOSING_SQL_TABLE_NAME;
    default:
      return "";
  }
}

Functionality::Idx Functionality::tableNameToIdx(const QString& tableName)
{
  if (tableName == IMAGE_SQL_TABLE_NAME)
    return Idx::Image;
  if (tableName == CATEGORY_SQL_TABLE_NAME)
    return Idx::Category;
  if (tableName == STORE_SQL_TABLE_NAME)
    return Idx::Store;
  if (tableName == PRODUCT_SQL_TABLE_NAME)
    return Idx::Product;
  if (tableName == PURCHASE_SQL_TABLE_NAME)
    return Idx::Purchase;
  if (tableName == USER_SQL_TABLE_NAME)
    return Idx::User;
  if (tableName == REMINDER_SQL_TABLE_NAME)
    return Idx::Reminder;
  if (tableName == SHOPPING_LIST_SQL_TABLE_NAME)
    return Idx::ShoppingList;
  if (tableName == EMPLOYEE_SQL_TABLE_NAME)
    return Idx::Employee;
  if (tableName == SUPPLIER_SQL_TABLE_NAME)
    return Idx::Supplier;
  if (tableName == COUPON_SQL_TABLE_NAME)
    return Idx::Coupon;
  if (tableName == PHONE_SQL_TABLE_NAME)
    return Idx::Phone;
  if (tableName == ADDRESS_SQL_TABLE_NAME)
    return Idx::Address;
  if (tableName == PRODUCT_CODE_ITEMS_SQL_TABLE_NAME)
    return Idx::ProductCode;
  if (tableName == INVENTORY_SQL_TABLE_NAME)
    return Idx::Inventory;
  if (tableName == LOGIN_SQL_TABLE_NAME)
    return Idx::Login;
  if (tableName == SECTOR_SQL_TABLE_NAME)
    return Idx::Sector;
  if (tableName == COIN_SQL_TABLE_NAME)
    return Idx::Coin;
  if (tableName == CASH_SQL_TABLE_NAME)
    return Idx::Cash;
  if (tableName == CASH_CLOSING_SQL_TABLE_NAME)
    return Idx::CashClosing;
  return Idx::_END;
}

QString Data::strMoney(double value, bool bCurrency)
{
  QString str(QString::number(value, 'f', 2));
  QStringList lst = str.split(".");
  if (lst.size() == 2 && lst.at(0).size() != 0)
  {
    int n = lst.at(0).size() / 3 + (lst.at(0).size() % 3 == 0 ? -1 : 0);
    int size = lst.at(0).size();
    for (int i = 1; i <= n; ++i)
      lst[0] = lst[0].insert(size - (i*3), ",");
    str = lst[0] + "." + lst[1];
  }
  return QString("%1%2").arg((bCurrency ? "R$" : ""), str);
}

QString Data::strAmmount(double value)
{
  return QString::number(value, 'f', 3);
}

QString Data::strFmt(double value)
{
  return QString::number(value, 'f').remove(QRegularExpression("\\.?0*$"));
}

QString Data::strInt(double value)
{
  return QString::number((int)value);
}

QString Data::strPercentage(double value)
{
  return QString::number(value, 'f', 2) + "%";
}

bool Data::areEqual(double v1, double v2, Type type)
{
  switch (type)
  {
    case Data::Type::Money:
      return (v1 < v2 + 0.01 && v1 > v2 - 0.01);
    default:
      return false;
  }
}

QString Data::str(double value, Type type)
{
  switch (type)
  {
    case Type::Ammount:
      return strAmmount(value);
    case Type::Fmt:
      return strFmt(value);
    case Type::Integer:
      return strInt(value);
    case Type::Money:
      return strMoney(value);
    case Type::Percentage:
      return strPercentage(value);
    default:
      return QString::number(value);
  }
}

QString Text::getRegEx(Input input)
{
  switch (input)
  {
    case Input::Alphanumeric:
      return "^[a-zA-Z0-9]*$";
    case Input::AlphanumericAndSpaces:
      return "^[a-zA-Z0-9 ]*$";
    case Input::Numeric:
      return "^[0-9]*$";
    case Input::BasicMath:
      return "^[0-9/\\-\\*\\+\\.][^,]*$";
    case Input::ASCII:
      return "[ -~]*";
    case Input::Alpha:
      return "^[a-zA-Z]*$";
    case Input::All:
    default:
      return "";
  }
}

QDateTime DateTime::server()
{
  QDateTime dt;
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  QSqlQuery query(db);
  query.prepare("SELECT NOW()");
  if (query.exec() && query.next())
    dt = query.value(0).toDateTime();
  return dt;
}
