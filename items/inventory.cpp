#include "inventory.h"

Inventory::Inventory()
{
  Inventory::clear();
}

void Inventory::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_products.clear();
  m_dt = QDateTime::currentDateTime();
  m_description.clear();
}

bool Inventory::operator != (const JItem& other) const
{
  const Inventory& another = dynamic_cast<const Inventory&>(other);
  return  m_products != another.m_products ||
         m_dt != another.m_dt ||
         m_description != another.m_description;
}

bool Inventory::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Inventory::isValid() const
{
  return !m_products.isEmpty();
}

QString Inventory::SQL_tableName() const
{
  return INVENTORY_SQL_TABLE_NAME;
}

bool Inventory::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " INVENTORY_SQL_TABLE_NAME " ("
                INVENTORY_SQL_COL_DAT ","
                INVENTORY_SQL_COL_DES ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02))");

  query.bindValue(":_v01", m_dt);
  query.bindValue(":_v02", m_description);

  bool ok = query.exec();
  if (ok)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_products.size() && ok; ++i)
    {
      m_products[i].m_ownerId = m_id;
      ok = m_products.at(i).SQL_insert_proc(query);
    }
  }

  return ok;
}

bool Inventory::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " INVENTORY_SQL_TABLE_NAME " SET "
                INVENTORY_SQL_COL_DAT " = (:_v01),"
                INVENTORY_SQL_COL_DES " = (:_v02)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_dt);
  query.bindValue(":_v02", m_description);

  bool ok = query.exec();
  if (ok)
  {
    ok = InventoryProduct::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_products.size() && ok; ++i)
    {
      m_products[i].m_ownerId = m_id;
      ok = m_products.at(i).SQL_insert_proc(query);
    }
  }
  return ok;
}

bool Inventory::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

  query.prepare("SELECT "
                INVENTORY_SQL_COL_DAT ","
                INVENTORY_SQL_COL_DES
                " FROM " INVENTORY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool ok = query.exec();
  if (ok)
  {
    if (query.next())
    {
      m_dt = query.value(0).toDateTime();
      m_description = query.value(1).toString();
    }
    else
    {
      error = "Entrada no estoque não encontrada.";
      ok = false;
    }
  }

  if (ok)
    ok = InventoryProduct::SQL_select_by_owner_id_proc(query, m_id, m_products, error);

  return ok;
}

bool Inventory::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " INVENTORY_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

static bool SQL_select_inventory(QVector<InventoryProduct>& products, QVector<double>& purchaseAmmount, QString& error)
{
  products.clear();
  purchaseAmmount.clear();
  error.clear();


}
