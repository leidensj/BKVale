#include "sqlitem.h"
#include "widgets/jdatabase.h"

Duo::Duo()
 : m_id(INVALID_ID)
{

}

Duo::Duo(Id id, const QString& name)
 : m_id(id)
 , m_name(name)
{

}

Duo::Duo(const QVariant& v)
{
  fromVariant(v);
}

Duo::Duo(const JItemSQL& o)
{
  m_id = o.m_id;
  m_name = o.name();
}

QVariant Duo::toVariant() const
{
  return st_toVariant(*this);
}

void Duo::fromVariant(const QVariant& v)
{
  *this = st_fromVariant(v);
}

QVariant Duo::st_toVariant(const Duo& o)
{
  QList<QVariant> lst;
  lst.push_back(o.m_id.get());
  lst.push_back(o.m_name);
  return QVariant(lst);
}

Duo Duo::st_fromVariant(const QVariant& v)
{
  Duo o;
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
      setValue(Duo(p->m_id, p->name()).toVariant());

  }
}

void SQLItem::evaluate()
{

}

void SQLItem::erase()
{
  setValue(Duo().toVariant());
}

void SQLItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v);
  setText(Duo(v).m_name);
}

void SQLItem::setTableName(const QString& tableName)
{
 m_tableName = tableName;
}

void SQLItem::setFilter(const QString& filter)
{
  m_filter = filter;
}
