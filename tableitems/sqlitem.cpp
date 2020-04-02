#include "sqlitem.h"
#include "controls/databaseselector.h"

SQLItem::SQLItem(const QString& tableName, const QString& filter)
  : m_tableName(tableName)
  , m_filter(filter)
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void SQLItem::activate()
{
  DatabaseSelector dlg(m_tableName, false, tableWidget());
  dlg.getViewer()->setFixedFilter(m_filter);
  if (dlg.exec())
  {
    JItemSQL* p = dlg.getViewer()->getCurrentItem();
    if (p != nullptr)
      setValue(SQLItem::st_toVariant(*p));
  }
}

void SQLItem::evaluate()
{

}

void SQLItem::erase()
{
  setValue(st_toVariant(Id(), ""));
}

void SQLItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, QVariant(v));
  setText(st_nameFromVariant(v));
}

QVariant SQLItem::st_toVariant(const JItemSQL& o)
{
  return st_toVariant(o.m_id, o.name());
}

QVariant SQLItem::st_toVariant(const Id& id, const QString& name)
{
  QList<QVariant> lst;
  lst.push_back(id.get());
  lst.push_back(name);
  return QVariant(lst);
}

QString SQLItem::st_nameFromVariant(const QVariant& v)
{
  QString name;
  for (int i = 0; i != v.toList().size(); ++i)
  {
    switch (i)
    {
      case 1:
        name = v.toList().at(i).toString();
        break;
      default:
        break;
    }
  }
  return name;
}

Id SQLItem::st_idFromVariant(const QVariant& v)
{
  Id id;
  for (int i = 0; i != v.toList().size(); ++i)
  {
    switch (i)
    {
      case 0:
        id.set(v.toList().at(i).toLongLong());
        break;
      default:
        break;
    }
  }
  return id;
}

void SQLItem::setTableName(const QString& tableName)
{
 m_tableName = tableName;
}

void SQLItem::setFilter(const QString& filter)
{
  m_filter = filter;
}
