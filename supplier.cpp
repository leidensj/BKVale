#include "supplier.h"
#include "jmodel.h"

class SupplierModel : public JModel
{
public:
  SupplierModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SUPPLIER_SQL_TABLE_NAME "." SQL_COLID ","
                     FORM_SQL_TABLE_NAME "." FORM_SQL_COL02 ","
                     FORM_SQL_TABLE_NAME "." FORM_SQL_COL03
                     " FROM " SUPPLIER_SQL_TABLE_NAME
                     " LEFT OUTER JOIN "
                     FORM_SQL_TABLE_NAME
                     " ON " SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL01 " = "
                     FORM_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select(getStrQuery());
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Nome"));
    setHeaderData(2, Qt::Horizontal, tr("Apelido"));
    if (header != nullptr)
    {
      if (header->count() == 3)
      {
        header->hideSection(0);
        header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
        header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      }
    }
  }
};

Supplier::Supplier()
{
  clear();
}

void Supplier::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_form.clear();
}

bool Supplier::operator !=(const JItem& other) const
{
  const Supplier& another = dynamic_cast<const Supplier&>(other);
  bool b =  m_form.m_id != another.m_form.m_id;
  return b;
}

bool Supplier::operator ==(const JItem& other) const
{
  return !(*this != other);
}

bool Supplier::isValid() const
{
  bool b = m_form.m_id.isValid();
  return b;
}

QString Supplier::SQL_tableName() const
{
  return SUPPLIER_SQL_TABLE_NAME;
}

bool Supplier::SQL_insert_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_insert_proc(query);
  if (bSuccess)
  {
    query.prepare("INSERT INTO " SUPPLIER_SQL_TABLE_NAME " ("
                  SUPPLIER_SQL_COL01 ")"
                  " VALUES ("
                  "(:_v01))");

    query.bindValue(":_v01", m_form.m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
      m_id.set(query.lastInsertId().toLongLong());
  }
  return bSuccess;
}

bool Supplier::SQL_update_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_update_proc(query);
  if (bSuccess)
  {
    query.prepare("UPDATE " SUPPLIER_SQL_TABLE_NAME " SET "
                  SUPPLIER_SQL_COL01 " = (:_v01)"
                  " WHERE " SQL_COLID " = (:_v00)");
    query.bindValue(":_v00", m_id.get());
    query.bindValue(":_v01", m_form.m_id.get());
    bSuccess = query.exec();
  }
  return bSuccess;
}

bool Supplier::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                SUPPLIER_SQL_COL01
                " FROM " SUPPLIER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_form.m_id.set(query.value(0).toLongLong());
    }
    else
    {
      error = "Fornecedor não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = m_form.SQL_select_proc(query, error);

  return bSuccess;
}

bool Supplier::SQL_remove_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_remove_proc(query);
  if (bSuccess)
  {
    query.prepare("DELETE FROM " SUPPLIER_SQL_TABLE_NAME " WHERE " SQL_COLID " = (:_v00)");
    query.bindValue(":_v00", m_id.get());
    bSuccess = query.exec();
  }
  return bSuccess;
}

JModel* Supplier::SQL_table_model(QObject *parent) const
{
  return new SupplierModel(parent);
}
