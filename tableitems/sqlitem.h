#ifndef SQLITEM_H
#define SQLITEM_H

#include "jtableitem.h"

class SQLItem : public JTableItem
{
public:
  SQLItem(const QString& tableName);
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

private:
  const QString& m_tableName;
};

#endif // SQLITEM_H
