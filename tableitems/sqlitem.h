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
   : m_id(id)
   , m_name(name)
  {

  }

  qlonglong m_id;
  QString m_name;
};

class SQLItem : public JTableItem
{
public:
  SQLItem(const QString& tableName, const QString& filter = "");
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

  static QVariant toVariant(const SQLItemAbv& o);
  static SQLItemAbv toSQLItemAbv(const QVariant& v);
  void setTableName(const QString& tableName);
  void setFilter(const QString& filter);
private:
  QString m_tableName;
  QString m_filter;
};

#endif // SQLITEM_H
