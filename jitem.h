#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include <QVector>
#include <QRegExp>

struct JItem
{
  enum class DataType
  {
    Integer,
    Money,
    Ammount,
    Fmt,
    Percentage
  };

  mutable qlonglong m_id;
  JItem() : m_id(INVALID_ID) {}
  virtual ~JItem() {}
  bool isValidId() const { return IS_VALID_ID(m_id); }
  virtual bool isValid() const { return true; }
  virtual void clear() { m_id = INVALID_ID; }
  virtual bool operator ==(const JItem& other) const { return m_id == other.m_id; }
  virtual bool operator !=(const JItem& other) const { return !(*this != other); }
  static QString st_strMoney(double value) { return "R$" + QString::number(value, 'f', 2); }
  static QString st_strAmmount(double value) { return QString::number(value, 'f', 3); }
  static QString st_strFmt(double value) { return QString::number(value, 'f').remove(QRegExp("\\.?0*$")); }
  static QString st_strInt(double value) { return QString::number((int)value); }
  static QString st_strPercentage(double value) { return QString::number(value, 'f', 2) + "%"; }

  static QString st_str(double value, DataType type)
  {
    switch (type)
    {
      case DataType::Ammount:
        return st_strAmmount(value);
      case DataType::Fmt:
        return st_strFmt(value);
      case DataType::Integer:
        return st_strInt(value);
      case DataType::Money:
        return st_strMoney(value);
      case DataType::Percentage:
        return st_strPercentage(value);
      default:
        return QString::number(value);
    }
  }
};

#endif // JITEM_H
