#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include "jtablecolumn.h"
#include <QVector>

struct JItem
{
  mutable qlonglong m_id;
  JItem() : m_id(INVALID_ID) {}
  bool isValidId() const { return IS_VALID_ID(m_id); }
  virtual bool isValid() { return IS_VALID_ID(m_id); }
  virtual void clear() { m_id = INVALID_ID; }
  virtual bool operator ==(const JItem& other) { return m_id == other.m_id; }
  virtual bool operator !=(const JItem& other) { return m_id != other.m_id; }
  static QVector<JTableColumn> getColumns() { return QVector<JTableColumn>(); }
};

#endif // JITEM_H
