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
  virtual bool isValid() const = 0;
  virtual void clear() = 0;
  virtual bool operator ==(const JItem& other) const = 0;
  virtual bool operator !=(const JItem& other) const = 0;
  virtual QString getStrName() const { return ""; }
  virtual QByteArray getArImage() const { return QByteArray(); }
  static QVector<JTableColumn> getColumns() { return QVector<JTableColumn>(); }
};

#endif // JITEM_H
