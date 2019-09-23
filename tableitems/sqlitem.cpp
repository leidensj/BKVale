#include "sqlitem.h"
#include "widgets/jdatabase.h"

QVariant SQLItem::toVariant(const SQLItemAbv& o)
{
  QList<QVariant> lst;
  lst.push_back(o.m_id);
  lst.push_back(o.m_name);
  return QVariant(lst);
}

SQLItemAbv SQLItem::toSQLItemAbv(const QVariant& v)
{
  SQLItemAbv o;
  for (int i = 0; i != v.toList().size(); ++i)
  {
    switch (i)
    {
      case 0:
        o.m_id = v.toList().at(i).toLongLong();
        break;
      case 1:
        o.m_name = v.toList().at(i).toString();
        break;
      default:
        break;
    }
  }
  return o;
}

SQLItem::SQLItem(const QString& tableName, const QString& filter)
  : m_tableName(tableName)
  , m_filter(filter)
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void SQLItem::activate()
{
  JDatabaseSelector dlg(m_tableName, false, tableWidget());
  dlg.getDatabase()->setFixedFilter(m_filter);
  if (dlg.exec())
  {
    JItemSQL* p = dlg.getDatabase()->getCurrentItem();
    if (p != nullptr)
      setValue(toVariant(SQLItemAbv(p->m_id.get(), p->name())));
  }
}

void SQLItem::evaluate()
{

}

void SQLItem::erase()
{
  setValue(toVariant(SQLItemAbv()));
}

void SQLItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v);
  setText(toSQLItemAbv(v).m_name);
}

void SQLItem::setTableName(const QString& tableName)
{
 m_tableName = tableName;
}

void SQLItem::setFilter(const QString& filter)
{
  m_filter = filter;
}
