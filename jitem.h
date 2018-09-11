#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include <QVector>
#include <QRegExp>

struct JItem
{
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
  static QString st_strInt(int value) { return QString::number(value); }
  static QString st_strPercentage(int value) { return QString::number(value, 'f', 2) + "%"; }
};

#endif // JITEM_H
