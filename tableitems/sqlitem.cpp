#include "sqlitem.h"
#include "widgets/jdatabase.h"

SQLItemId::SQLItemId()
 : m_id(INVALID_ID)
{

}

SQLItemId::SQLItemId(Id id, const QString& name)
 : m_id(id)
 , m_name(name)
{

}

SQLItemId::SQLItemId(const QVariant& v)
{
  fromVariant(v);
}

QVariant SQLItemId::toVariant() const
{
  return st_toVariant(*this);
}

void SQLItemId::fromVariant(const QVariant& v)
{
  *this = st_fromVariant(v);
}

QVariant SQLItemId::st_toVariant(const SQLItemId& o)
{
  QList<QVariant> lst;
  lst.push_back(o.m_id.get());
  lst.push_back(o.m_name);
  return QVariant(lst);
}

SQLItemId SQLItemId::st_fromVariant(const QVariant& v)
{
  SQLItemId o;
  for (int i = 0; i != v.toList().size(); ++i)
  {
    switch (i)
    {
      case 0:
        o.m_id.set(v.toList().at(i).toLongLong());
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
      setValue(SQLItemId(p->m_id, p->name()).toVariant());

  }
}

void SQLItem::evaluate()
{

}

void SQLItem::erase()
{
  setValue(SQLItemId().toVariant());
}

void SQLItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v);
  setText(SQLItemId(v).m_name);
}

void SQLItem::setTableName(const QString& tableName)
{
 m_tableName = tableName;
}

void SQLItem::setFilter(const QString& filter)
{
  m_filter = filter;
}
