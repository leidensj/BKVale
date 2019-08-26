#include "common.h"

QString Data::strMoney(double value)
{
  return "R$" + QString::number(value, 'f', 2);
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
    case Input::All:
    default:
      return "";
  }
}
