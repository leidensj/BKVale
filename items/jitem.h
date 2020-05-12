#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include "common.h"
#include <QRegExp>
#include <QVariant>
#include <QVector>

class Id
{
private:
  qlonglong m_id;

public:
  static bool st_isValid(qlonglong id) { return id != INVALID_ID; }
  bool isValid() const { return st_isValid(m_id); }
  void clear() { m_id = INVALID_ID; }
  void set(qlonglong id) { m_id = id > INVALID_ID ? id: INVALID_ID; }
  QString str() const { return QString::number(m_id); }
  qlonglong get() const { return m_id; }
  QVariant getIdNull() const { return isValid() ? m_id : QVariant(QVariant::LongLong); }
  Id() { clear(); }
  Id(qlonglong id) : m_id(id) { }
  bool operator ==(const Id& other) const { return m_id == other.m_id; }
  bool operator !=(const Id& other) const { return !(*this == other); }
};

struct JItem
{
  mutable Id m_id;
  virtual ~JItem() { }
  virtual bool isValid() const = 0;
  virtual void clear(bool bClearId) = 0;
  virtual bool operator ==(const JItem& other) const = 0;
  virtual bool operator !=(const JItem& other) const = 0;
};

typedef QVector<Id> Ids;
typedef QVector<QString> Names;

#endif // JITEM_H
