#ifndef STOCKAISLE_H
#define STOCKAISLE_H

#include "jitem.h"
#include "product.h"
#include "person.h"

struct Stock : public JItem
{
  Product m_product;
  double m_ammount;
  QDateTime m_timestamp;
  Person m_employee;
  qlonglong m_noteId;

  void clear()
  {
    m_id = INVALID_ID;
    m_product.clear();
    m_ammount = 0.0;
    m_timestamp = QDateTime::currentDateTime();
    m_employee.clear();
    m_noteId = INVALID_ID;
  }

  Stock()
  {
    clear();
  }

  bool operator != (const JItem& other) const
  {
    const Stock& another = dynamic_cast<const Stock&>(other);
    return m_product.m_id != another.m_product.m_id ||
           m_ammount != another.m_ammount ||
           m_timestamp != another.m_timestamp ||
           m_employee.m_id != another.m_employee.m_id ||
           m_noteId != another.m_noteId;
  }

  bool operator == (const JItem& other) const
  {
    return !(*this != other);
  }

  QString strTableName() const
  {
    return STOCK_SQL_TABLE_NAME;
  }
};

#endif // STOCKAISLE_H
