#ifndef SQLITEM_H
#define SQLITEM_H

#include "jtableitem.h"
#include "items/jitemsql.h"

struct SQLItemId
{
  Id m_id;
  QString m_name;

  SQLItemId();
  SQLItemId(Id id, const QString& name);
  SQLItemId(const QVariant& v);
  QVariant toVariant() const;
  void fromVariant(const QVariant& v);

  static QVariant st_toVariant(const SQLItemId& o);
  static SQLItemId st_fromVariant(const QVariant& v);
};

class SQLItem : public JTableItem
{
public:
  SQLItem(const QString& tableName, const QString& filter = "");
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

  void setTableName(const QString& tableName);
  void setFilter(const QString& filter);

private:
  QString m_tableName;
  QString m_filter;
};

#endif // SQLITEM_H
