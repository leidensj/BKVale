#ifndef JITEM_H
#define JITEM_H

#include "defines.h"
#include "jtablecolumn.h"
#include <QVector>

struct JItem
{
  mutable qlonglong m_id;
  JItem() : m_id(INVALID_ID) {}
  virtual ~JItem() {}
  bool isValidId() const { return IS_VALID_ID(m_id); }
  virtual bool isValid() const { return isValidId(); }
  virtual void clear() { m_id = INVALID_ID; }
  virtual bool operator ==(const JItem& other) const { return m_id == other.m_id; }
  virtual bool operator !=(const JItem& other) const { return !(*this != other); }
};

#endif // JITEM_H
