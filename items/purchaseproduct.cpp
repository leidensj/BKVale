#include "purchaseproduct.h"

PurchaseProduct::PurchaseProduct()
{
  PurchaseProduct::clear();
}

void PurchaseProduct::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
  m_price = 0.0;
  m_package.clear();
  m_ownerId.clear();
}

bool PurchaseProduct::isValid() const
{
  return true;
}

bool PurchaseProduct::operator !=(const JItem& other) const
{
  const PurchaseProduct& another = dynamic_cast<const PurchaseProduct&>(other);
  return
      m_product.m_id != another.m_product.m_id ||
      m_ammount != another.m_ammount ||
      m_price != another.m_price ||
      m_package != another.m_package;
}

bool PurchaseProduct::operator ==(const JItem& other) const
{
  return !(*this != other);
}

double PurchaseProduct::subtotal() const
{
  return m_ammount * m_price;
}

QString PurchaseProduct::strSubtotal() const
{
  return Data::strMoney(subtotal());
}

QString PurchaseProduct::strAmmount() const
{
  return Data::strAmmount(m_ammount);
}

QString PurchaseProduct::strPrice() const
{
  return Data::strMoney(m_price);
}

bool PurchaseProduct::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " PURCHASE_ELEMENTS_SQL_TABLE_NAME " ("
                PURCHASE_ELEMENTS_SQL_COL_NID ","
                PURCHASE_ELEMENTS_SQL_COL_PID ","
                PURCHASE_ELEMENTS_SQL_COL_AMM ","
                PURCHASE_ELEMENTS_SQL_COL_PRI ","
                PURCHASE_ELEMENTS_SQL_COL_ISP ","
                PURCHASE_ELEMENTS_SQL_COL_PUN ","
                PURCHASE_ELEMENTS_SQL_COL_PAM
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_product.m_id.get());
  query.bindValue(":_v03", m_ammount);
  query.bindValue(":_v04", m_price);
  query.bindValue(":_v05", m_package.m_bIsPackage);
  query.bindValue(":_v06", m_package.m_unity);
  query.bindValue(":_v07", m_package.m_ammount);
  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool PurchaseProduct::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<PurchaseProduct>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                PURCHASE_ELEMENTS_SQL_COL_PID ","
                PURCHASE_ELEMENTS_SQL_COL_AMM ","
                PURCHASE_ELEMENTS_SQL_COL_PRI ","
                PURCHASE_ELEMENTS_SQL_COL_ISP ","
                PURCHASE_ELEMENTS_SQL_COL_PUN ","
                PURCHASE_ELEMENTS_SQL_COL_PAM
                " FROM " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                " WHERE " PURCHASE_ELEMENTS_SQL_COL_NID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    while (query.next())
    {
      PurchaseProduct o;
      o.m_id.set(query.value(0).toLongLong());
      o.m_product.m_id.set(query.value(1).toLongLong());
      o.m_ammount = query.value(2).toDouble();
      o.m_price = query.value(3).toDouble();
      o.m_package.m_bIsPackage = query.value(4).toBool();
      o.m_package.m_unity = query.value(5).toString();
      o.m_package.m_ammount = query.value(6).toDouble();
      v.push_back(o);
    }
  }

  for (int i = 0; i != v.size() && bSuccess; ++i)
    if (v.at(i).m_product.m_id.isValid())
      bSuccess = v[i].m_product.SQL_select_proc(query, error);

  return bSuccess;
}

bool PurchaseProduct::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                PURCHASE_ELEMENTS_SQL_COL_NID ","
                PURCHASE_ELEMENTS_SQL_COL_PID ","
                PURCHASE_ELEMENTS_SQL_COL_AMM ","
                PURCHASE_ELEMENTS_SQL_COL_PRI ","
                PURCHASE_ELEMENTS_SQL_COL_ISP ","
                PURCHASE_ELEMENTS_SQL_COL_PUN ","
                PURCHASE_ELEMENTS_SQL_COL_PAM
                " FROM " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_product.m_id.set(query.value(1).toLongLong());
      m_ammount = query.value(2).toDouble();
      m_price = query.value(3).toDouble();
      m_package.m_bIsPackage = query.value(4).toBool();
      m_package.m_unity = query.value(5).toString();
      m_package.m_ammount = query.value(6).toDouble();
    }
    else
    {
      error = "Item da compra não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = m_product.SQL_select_proc(query, error);

  return bSuccess;
}

void PurchaseProduct::SQL_select_last(Id supplierId, Id productId)
{
  QString error;
  if (!JItemSQL::SQL_isOpen(error))
    return;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                PURCHASE_ELEMENTS_SQL_TABLE_NAME "." SQL_COLID
                " FROM " PURCHASE_SQL_TABLE_NAME
                " INNER JOIN " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                " ON " PURCHASE_SQL_TABLE_NAME "." SQL_COLID
                " = " PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_NID
                " WHERE " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SID
                " = (:_v01)"
                " AND " PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID
                " = (:_v02) "
                " ORDER BY " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DAT
                " DESC LIMIT 1");
  query.bindValue(":_v01", supplierId.get());
  query.bindValue(":_v02", productId.get());
  bool bSuccess = query.exec();

  if (bSuccess && query.next())
  {
    m_id.set(query.value(0).toLongLong());
    bSuccess = SQL_select_proc(query, error);
  }

  JItemSQL::SQL_finish(db, query, bSuccess, error);
}

void PurchaseProduct::SQL_select_last(Id productId)
{
  QString error;
  if (!JItemSQL::SQL_isOpen(error))
    return;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                PURCHASE_ELEMENTS_SQL_TABLE_NAME "." SQL_COLID
                " FROM " PURCHASE_SQL_TABLE_NAME
                " INNER JOIN " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                " ON " PURCHASE_SQL_TABLE_NAME "." SQL_COLID
                " = " PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_NID
                " WHERE " PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID
                " = (:_v01) "
                " ORDER BY " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DAT
                ", " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_NUM
                " DESC LIMIT 1");
  query.bindValue(":_v01", productId.get());
  bool bSuccess = query.exec();

  if (bSuccess && query.next())
  {
    m_id.set(query.value(0).toLongLong());
    bSuccess = SQL_select_proc(query, error);
  }

  JItemSQL::SQL_finish(db, query, bSuccess, error);
}

bool PurchaseProduct::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                " WHERE " PURCHASE_ELEMENTS_SQL_COL_NID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
