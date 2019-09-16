#include "sqlitem.h"
#include "widgets/jdatabase.h"

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
    {
      setData(Qt::UserRole, p->m_id.get());
      setText(p->name());
    }
  }
}

void SQLItem::evaluate()
{

}

void SQLItem::erase()
{
  setData(Qt::UserRole, INVALID_ID);
  setText("");
}

void SQLItem::setValue(const QVariant& v)
{
  setData(Qt::UserRole, v.toLongLong());
}
