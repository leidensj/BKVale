#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include <QRegExp>

class Id
{
private:
  qlonglong m_id;

public:
  static bool st_isValid(qlonglong id) { return id != INVALID_ID; }
  static QString st_str(qlonglong id) { return QString::number(id); }
  bool isValid() const { return st_isValid(m_id); }
  void clear() { m_id = INVALID_ID; }
  void set(qlonglong id) { m_id = id; }
  QString str() const { return st_str(m_id); }
  qlonglong get() const { return m_id; }
  Id() { clear(); }
  Id(qlonglong id) : m_id(id) { }
  bool operator ==(const Id& other) const { return m_id == other.m_id; }
  bool operator !=(const Id& other) const { return !(*this == other); }
};

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

  virtual ~JItem() {}
  mutable Id m_id;
  virtual bool isValid() const { return true; }
  virtual void clear() { m_id.clear(); }
  virtual QString strTableName() const { return ""; }
  virtual bool operator ==(const JItem& other) const { return m_id == other.m_id; }
  virtual bool operator !=(const JItem& other) const { return !(*this == other); }
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
