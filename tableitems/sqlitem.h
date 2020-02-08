#ifndef SQLITEM_H
#define SQLITEM_H

#include "jtableitem.h"
#include "items/jitemsql.h"

class SQLItem : public JTableItem
{
public:
  SQLItem(const QString& tableName, const QString& filter = "");
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);
  static QVariant st_toVariant(const JItemSQL& o);
  static QVariant st_toVariant(const Id& id, const QString& name);
  static QString st_nameFromVariant(const QVariant& v);
  static Id st_idFromVariant(const QVariant& v);

  void setTableName(const QString& tableName);
  void setFilter(const QString& filter);

private:
  QString m_tableName;
  QString m_filter;
};

#endif // SQLITEM_H
