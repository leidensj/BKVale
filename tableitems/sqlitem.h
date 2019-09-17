#ifndef SQLITEM_H
#define SQLITEM_H

#include "jtableitem.h"
#include "items/jitemsql.h"

struct SQLItemAbv
{
  SQLItemAbv()
   : m_id(INVALID_ID)
  {

  }

  SQLItemAbv(qlonglong id, const QString& name)
   : m_id(INVALID_ID)
   , m_name(name)
  {

  }

  qlonglong m_id;
  QString m_name;
};

class SQLItem : public JTableItem
{
public:
  SQLItem(const QString& tableName);
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

  static QVariant toVariant(const SQLItemAbv& o);
  static SQLItemAbv toSQLItemAbv(const QVariant& v);
private:
  const QString& m_tableName;
};

#endif // SQLITEM_H
