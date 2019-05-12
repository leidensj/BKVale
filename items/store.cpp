#include "store.h"
#include "jmodel.h"

class StoreModel : public JModel
{
public:
  StoreModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     STORE_SQL_TABLE_NAME "." SQL_COLID ","
                     FORM_SQL_TABLE_NAME "." FORM_SQL_COL02 ","
                     FORM_SQL_TABLE_NAME "." FORM_SQL_COL03 ","
                     STORE_SQL_TABLE_NAME "." STORE_SQL_COL04
                     " FROM "
                     STORE_SQL_TABLE_NAME
                     " LEFT OUTER JOIN "
                     FORM_SQL_TABLE_NAME
                     " ON " STORE_SQL_TABLE_NAME "." STORE_SQL_COL01
                     " = " FORM_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Razão Social"));
    setHeaderData(2, Qt::Horizontal, tr("Nome Fantasia"));
    setHeaderData(3, Qt::Horizontal, tr("Descrição"));
    if (header != nullptr && header->count() == 4)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    }
  }
};

Store::Store()
{
  clear();
}

void Store::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_form.clear(bClearId);
  m_address.clear();
  m_phone.clear();
  m_description.clear();
  m_vEmployee.clear();
}

bool Store:: operator != (const JItem& other) const
{
  const Store& another = dynamic_cast<const Store&>(other);
  return m_form.m_id != another.m_form.m_id ||
         m_address.m_id != another.m_address.m_id ||
         m_phone.m_id != another.m_phone.m_id ||
         m_description != another.m_description ||
         m_vEmployee != another.m_vEmployee;
}

bool Store:: operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Store::isValid() const
{
  return m_form.m_id.isValid();
}

QString Store::SQL_tableName() const
{
  return STORE_SQL_TABLE_NAME;
}

bool Store::SQL_insert_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_insert_proc(query);
  if (bSuccess)
  {
    query.prepare("INSERT INTO " STORE_SQL_TABLE_NAME " ("
                  STORE_SQL_COL01 ","
                  STORE_SQL_COL02 ","
                  STORE_SQL_COL03 ","
                  STORE_SQL_COL04 ")"
                  " VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04))");

    query.bindValue(":_v01", m_form.m_id.getIdNull());
    query.bindValue(":_v02", m_address.m_id.getIdNull());
    query.bindValue(":_v03", m_phone.m_id.getIdNull());
    query.bindValue(":_v04", m_description);

    bSuccess = query.exec();
    if (bSuccess)
    {
      m_id.set(query.lastInsertId().toLongLong());
      for (int i = 0; i != m_vEmployee.size() && bSuccess; ++i)
      {
        query.prepare("INSERT INTO " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                      STORE_EMPLOYEES_SQL_COL01 ","
                      STORE_EMPLOYEES_SQL_COL02 ")"
                      " VALUES ("
                      "(:_v01),"
                      "(:_v02))");
        query.bindValue(":_v01", m_vEmployee.at(i).m_id.get());
        query.bindValue(":_v02", m_id.get());
        bSuccess = query.exec();
      }
    }
  }
  return bSuccess;
}

bool Store::SQL_update_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_update_proc(query);
  if (bSuccess)
  {
    query.prepare("UPDATE " STORE_SQL_TABLE_NAME " SET "
                  STORE_SQL_COL01 " = (:_v01),"
                  STORE_SQL_COL02 " = (:_v02),"
                  STORE_SQL_COL03 " = (:_v03),"
                  STORE_SQL_COL04 " = (:_v04)"
                  " WHERE " SQL_COLID " = (:_v00)");

    query.bindValue(":_v00", m_id.get());
    query.bindValue(":_v01", m_form.m_id.getIdNull());
    query.bindValue(":_v02", m_address.m_id.getIdNull());
    query.bindValue(":_v03", m_phone.m_id.getIdNull());
    query.bindValue(":_v04", m_description);
    bSuccess = query.exec();
    if (bSuccess)
    {
      query.prepare("DELETE FROM " STORE_EMPLOYEES_SQL_TABLE_NAME
                    " WHERE " STORE_EMPLOYEES_SQL_COL02 " = (:_v02)");
      query.bindValue(":_v02", m_id.get());
      bSuccess = query.exec();
      for (int i = 0; i != m_vEmployee.size() && bSuccess; ++i)
      {
        query.prepare("INSERT INTO " STORE_EMPLOYEES_SQL_TABLE_NAME " ("
                      STORE_EMPLOYEES_SQL_COL01 ","
                      STORE_EMPLOYEES_SQL_COL02 ")"
                      " VALUES ("
                      "(:_v01),"
                      "(:_v02))");
        query.bindValue(":_v01", m_vEmployee.at(i).m_id.get());
        query.bindValue(":_v02", m_id.get());
        bSuccess = query.exec();
      }
    }
  }

  return bSuccess;
}

bool Store::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                STORE_SQL_COL01 ","
                STORE_SQL_COL02 ","
                STORE_SQL_COL03 ","
                STORE_SQL_COL04
                " FROM " STORE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_form.m_id = query.value(0).toLongLong();
      m_address.m_id = query.value(1).toLongLong();
      m_phone.m_id = query.value(2).toLongLong();
      m_description = query.value(3).toString();
    }
    else
    {
      error = "Loja não encontrada.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  STORE_EMPLOYEES_SQL_COL01
                  " FROM " STORE_EMPLOYEES_SQL_TABLE_NAME
                  " WHERE " STORE_EMPLOYEES_SQL_COL02 " = (:_v02)");
    query.bindValue(":_v02", m_id.get());
    bSuccess = query.exec();
    while (bSuccess && query.next())
    {
      Employee e;
      e.m_id.set(query.value(0).toLongLong());
      m_vEmployee.push_back(e);
    }
  }

  for (int i = 0; i != m_vEmployee.size() && bSuccess; ++i)
    bSuccess = m_vEmployee[i].SQL_select_proc(query, error);

  if (bSuccess && m_form.m_id.isValid())
    bSuccess = m_form.SQL_select_proc(query, error);

  if (bSuccess && m_phone.m_id.isValid())
    bSuccess = m_phone.SQL_select_proc(query, error);

  if (bSuccess && m_address.m_id.isValid())
    bSuccess = m_address.SQL_select_proc(query, error);

  return bSuccess;
}

bool Store::SQL_remove_proc(QSqlQuery& query) const
{
  bool bSuccess = m_form.SQL_remove_proc(query);
  if (bSuccess)
  {
    query.prepare("DELETE FROM " STORE_SQL_TABLE_NAME
                  " WHERE " SQL_COLID " = (:_v00)");
    query.bindValue(":_v00", m_id.get());
    bSuccess = query.exec();
  }
  return bSuccess;
}

JModel* Store::SQL_table_model(QObject* parent) const
{
  return new StoreModel(parent);
}
