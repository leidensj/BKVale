#include "category.h"
#include "jmodel.h"

class CategoryModel : public JModel
{
public:
  CategoryModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     CATEGORY_SQL_COL02
                     " FROM "
                     CATEGORY_SQL_TABLE_NAME);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    if (header != nullptr && header->count() == 2)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    }
  }
};

Category::Category()
{
  clear();
}

void Category::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_name.clear();
  m_image.clear();
}

bool Category::operator != (const JItem& other) const
{
  const Category& another = dynamic_cast<const Category&>(other);
  return m_name != another.m_name ||
         m_image.m_id != another.m_image.m_id;
}

bool Category::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Category::isValid() const
{
  return !m_name.isEmpty();
}

QString Category::SQL_tableName() const
{
  return CATEGORY_SQL_TABLE_NAME;
}

bool Category::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " CATEGORY_SQL_TABLE_NAME " ("
                CATEGORY_SQL_COL01 ","
                CATEGORY_SQL_COL02 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");

  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

  return bSuccess;
}

bool Category::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " CATEGORY_SQL_TABLE_NAME " SET "
                CATEGORY_SQL_COL01 " = (:_v01),"
                CATEGORY_SQL_COL02 " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_image.m_id.getIdNull());
  query.bindValue(":_v02", m_name);

  return query.exec();
}

bool Category::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                CATEGORY_SQL_COL01 ","
                CATEGORY_SQL_COL02
                " FROM " CATEGORY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_image.m_id.set(query.value(0).toLongLong());
      m_name = query.value(1).toString();
    }
    else
    {
      error = "Categoria não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess && m_image.m_id.isValid())
    bSuccess = m_image.SQL_select_proc(query, error);

  return bSuccess;
}

bool Category::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " CATEGORY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

JModel* Category::SQL_table_model(QObject* parent) const
{
  return new CategoryModel(parent);
}
