#include "couponproduct.h"

CouponProduct::CouponProduct()
{
  CouponProduct::clear();
}

void CouponProduct::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
}

bool CouponProduct::isValid() const
{
  return m_product.m_id.isValid() && m_ammount >= 0.0;
}

bool CouponProduct::operator !=(const JItem& other) const
{
  const CouponProduct& another = dynamic_cast<const CouponProduct&>(other);
  return
      m_product.m_id != another.m_product.m_id ||
      m_ammount != another.m_ammount;
}

bool CouponProduct::operator ==(const JItem& other) const
{
  return !(*this != other);
}

QString CouponProduct::strAmmount() const
{
  return Data::strAmmount(m_ammount);
}

QString CouponProduct::strFmt() const
{
  return Data::strFmt(m_ammount);
}

bool CouponProduct::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " COUPON_ELEMENTS_SQL_TABLE_NAME " ("
                COUPON_ELEMENTS_SQL_COL_CID ","
                COUPON_ELEMENTS_SQL_COL_PID ","
                COUPON_ELEMENTS_SQL_COL_AMM
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_product.m_id.get());
  query.bindValue(":_v03", m_ammount);
  bool bSuccess = query.exec();

  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool CouponProduct::SQL_select_by_owner_id_proc(QSqlQuery& query, Id ownerId, QVector<CouponProduct>& v, QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                COUPON_ELEMENTS_SQL_COL_PID ","
                COUPON_ELEMENTS_SQL_COL_AMM
                " FROM " COUPON_ELEMENTS_SQL_TABLE_NAME
                " WHERE " COUPON_ELEMENTS_SQL_COL_CID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    Ids ids;
    while (query.next())
      ids.push_back(query.value(0).toLongLong());
    for (int i = 0; i != ids.size(); ++i)
    {
      CouponProduct o;
      o.m_id = ids.at(i);
      if (o.SQL_select_proc(query, error))
        v.push_back(o);
    }
  }

  for (int i = 0; i != v.size() && bSuccess; ++i)
    if (v.at(i).m_product.m_id.isValid())
      bSuccess = v[i].m_product.SQL_select_proc(query, error);

  return bSuccess;
}

bool CouponProduct::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                COUPON_ELEMENTS_SQL_COL_CID ","
                COUPON_ELEMENTS_SQL_COL_PID ","
                COUPON_ELEMENTS_SQL_COL_AMM
                " FROM " COUPON_ELEMENTS_SQL_TABLE_NAME
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
    }
    else
    {
      error = "Item do cupon não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = m_product.SQL_select_proc(query, error);

  return bSuccess;
}

bool CouponProduct::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " COUPON_ELEMENTS_SQL_TABLE_NAME
                " WHERE " COUPON_ELEMENTS_SQL_COL_CID " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}
