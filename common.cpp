#include "common.h"
#include "defines.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

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
  return QString::number(value, 'f').remove(QRegExp("\\.?0*$"));
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
  QDateTime dt(QDateTime::currentDateTime());
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  QSqlQuery query(db);
  query.prepare("SELECT NOW()");
  if (query.exec() && query.next())
    dt = query.value(0).toDateTime();
  return dt;
}
