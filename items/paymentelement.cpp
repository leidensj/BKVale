#include "paymentelement.h"

PaymentElement::PaymentElement()
{
  clear();
}

void PaymentElement::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_value = 0.0;
  m_date = QDate::currentDate();
  m_ownerId.clear();
}

bool PaymentElement::operator != (const JItem& other) const
{
  const PaymentElement& another = dynamic_cast<const PaymentElement&>(other);
  return m_value != another.m_value ||
         m_date != another.m_date;
}

bool PaymentElement::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool PaymentElement::isValid() const
{
  return true;
}

bool PaymentElement::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME " ("
                NOTE_PAYMENT_ITEMS_SQL_COL01 ","
                NOTE_PAYMENT_ITEMS_SQL_COL02 ","
                NOTE_PAYMENT_ITEMS_SQL_COL03
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");
  query.bindValue(":_v01", m_ownerId.get());
  query.bindValue(":_v02", m_date);
  query.bindValue(":_v03", m_value);
  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool PaymentElement::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  query.prepare("SELECT "
                NOTE_PAYMENT_ITEMS_SQL_COL01 ","
                NOTE_PAYMENT_ITEMS_SQL_COL02 ","
                NOTE_PAYMENT_ITEMS_SQL_COL03
                " FROM " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_ownerId.set(query.value(0).toLongLong());
      m_date = query.value(1).toDate();
      m_value = query.value(2).toDouble();
    }
    else
    {
      error = "Pagamento não encontrado.";
      bSuccess = false;
    }
  }
  return bSuccess;
}

bool PaymentElement::SQL_select_by_owner_id_proc(QSqlQuery& query,
                                                 Id ownerId,
                                                 QVector<PaymentElement>& v,
                                                 QString& error)
{
  error.clear();
  v.clear();
  query.prepare("SELECT "
                SQL_COLID ","
                NOTE_PAYMENT_ITEMS_SQL_COL02 ","
                NOTE_PAYMENT_ITEMS_SQL_COL03
                " FROM " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME
                " WHERE " NOTE_PAYMENT_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  bool bSuccess = query.exec();
  if (bSuccess)
  {
    while (query.next())
    {
      PaymentElement o;
      o.m_id.set(query.value(0).toLongLong());
      o.m_date = query.value(1).toDate();
      o.m_value = query.value(2).toDouble();
      o.m_ownerId = ownerId;
      v.push_back(o);
    }
  }
  return bSuccess;
}

bool PaymentElement::SQL_remove_by_owner_id_proc(QSqlQuery& query, Id ownerId)
{
  query.prepare("DELETE FROM " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME
                " WHERE " NOTE_PAYMENT_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", ownerId.get());
  return query.exec();
}

