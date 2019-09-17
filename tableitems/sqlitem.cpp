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

SQLItem::SQLItem(const QString& tableName)
  : m_tableName(tableName)
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void SQLItem::activate()
{
  JDatabaseSelector dlg(m_tableName, false, tableWidget());
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
  setValue(SQLItemAbv());
}

void SQLItem::setValue(const QVariant& v)
{
  SQLItemAbv abv = toSQLItemAbv(v);
  setData(Qt::UserRole, abv.m_id);
  setText(abv.m_name);
}
