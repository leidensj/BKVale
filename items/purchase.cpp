#include "purchase.h"

PaymentItem::PaymentItem()
{
  clear();
}

void PaymentItem::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_value = 0.0;
  m_date = QDate::currentDate();
}

bool PaymentItem::operator != (const JItem& other) const
{
  const PaymentItem& another = dynamic_cast<const PaymentItem&>(other);
  return m_value != another.m_value ||
         m_date != another.m_date;
}

bool PaymentItem::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool PaymentItem::isValid() const
{
  return true;
}

Purchase::Purchase(Id id)
{
  clear();
  m_id = id;
}

void Purchase::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_paymentMethod = PaymentMethod::Credit;
  m_vPaymentItem.clear();
  m_number = 0;
  m_supplier.clear();
  m_date = QDate::currentDate();
  m_vItem.clear();
  m_observation.clear();
  m_disccount = 0.0;
  m_employee.clear();
}

bool Purchase::isValid() const
{
  return true;
}

bool Purchase::operator !=(const JItem& other) const
{
  const Purchase& another = dynamic_cast<const Purchase&>(other);
  return
      m_number != another.m_number ||
      m_date != another.m_date ||
      m_supplier.m_id != another.m_supplier.m_id ||
      m_paymentMethod != another.m_paymentMethod ||
      m_vPaymentItem != another.m_vPaymentItem ||
      m_vItem != another.m_vItem ||
      m_disccount != another.m_disccount;
}

bool Purchase::operator ==(const JItem& other) const
{
  return !(*this != other);
}

QString Purchase::strDate() const
{
  return m_date.toString("dd/MM/yyyy");
}

QString Purchase::strDayOfWeek() const
{
  return m_date.toString("dddd");
}

QString Purchase::strNumber() const
{
  return st_strInt(m_number);
}

QString Purchase::strDisccount() const
{
  return st_strMoney(m_disccount);
}

double Purchase::total() const
{
  double total = 0.0;
  for (int i = 0; i != m_vItem.size(); ++i)
    total += m_vItem.at(i).subtotal();
  return total + m_disccount;
}

double Purchase::subTotal() const
{
  double subTotal = 0.0;
  for (int i = 0; i != m_vItem.size(); ++i)
    subTotal += m_vItem.at(i).subtotal();
  return subTotal;
}

QString Purchase::strTotal() const
{
  return st_strMoney(total());
}

QString Purchase::strSubTotal() const
{
  return st_strMoney(subTotal());
}

bool Purchase::isPaymentValid() const
{
  if (m_paymentMethod == PaymentMethod::Credit)
  {
    bool bValid = true;
    for (int i = 0; i != m_vPaymentItem.size() && bValid; ++i)
      bValid = m_vPaymentItem.at(i).m_date >= m_date;
    return bValid && st_areEqual(total(), paymentTotal(), DataType::Money);
  }
  return true;
}

double Purchase::paymentTotal() const
{
  if (m_paymentMethod == PaymentMethod::Credit)
  {
    double t = 0.0;
    for (int i = 0; i != m_vPaymentItem.size(); ++i)
      t += m_vPaymentItem.at(i).m_value;
    return t;
  }
  return total();
}

void Purchase::adjustPayment()
{
  if (!isPaymentValid())
  {
    m_vPaymentItem.clear();
    m_paymentMethod = PaymentMethod::Credit;
    PaymentItem o;
    o.m_value = total();
    o.m_date = m_date.addMonths(1);
    m_vPaymentItem.push_back(o);
  }
}

QString Purchase::SQL_tableName() const
{
  return NOTE_SQL_TABLE_NAME;
}

bool Purchase::SQL_insert_proc(QSqlQuery& query) const
{
  bool bSuccess = query.exec("SELECT MAX(" NOTE_SQL_COL01 ") FROM " NOTE_SQL_TABLE_NAME);
  if (bSuccess)
  {
    m_number = query.next()
               ? query.value(0).toLongLong() + 1
               : NOTE_DEFAULT_NUMBER;
    m_number = m_number > NOTE_DEFAULT_NUMBER ? m_number : NOTE_DEFAULT_NUMBER;

    query.prepare("INSERT INTO " NOTE_SQL_TABLE_NAME " ("
                  NOTE_SQL_COL01 ","
                  NOTE_SQL_COL02 ","
                  NOTE_SQL_COL03 ","
                  NOTE_SQL_COL04 ","
                  NOTE_SQL_COL05 ","
                  NOTE_SQL_COL06 ","
                  NOTE_SQL_COL07
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04),"
                  "(:_v05),"
                  "(:_v06),"
                  "(:_v07))");
    query.bindValue(":_v01", m_number);
    query.bindValue(":_v02", m_date);
    query.bindValue(":_v03", m_supplier.m_id.getIdNull());
    query.bindValue(":_v04", m_observation);
    query.bindValue(":_v05", m_disccount);
    query.bindValue(":_v06", m_employee.m_id.getIdNull());
    query.bindValue(":_v07", (int)m_paymentMethod);
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vItem.size() && bSuccess; ++i)
    {
      m_vItem[i].m_ownerId = m_id;
      bSuccess = m_vItem.at(i).SQL_insert_proc(query);
    }
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vPaymentItem.size() && bSuccess; ++i)
    {
      query.prepare("INSERT INTO " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME " ("
                    NOTE_PAYMENT_ITEMS_SQL_COL01 ","
                    NOTE_PAYMENT_ITEMS_SQL_COL02 ","
                    NOTE_PAYMENT_ITEMS_SQL_COL03
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vPaymentItem.at(i).m_date);
      query.bindValue(":_v03", m_vPaymentItem.at(i).m_value);
      bSuccess = query.exec();
      if (bSuccess)
        m_vPaymentItem.at(i).m_id.set(query.lastInsertId().toLongLong());
    }
  }

  return bSuccess;
}

bool Purchase::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " NOTE_SQL_TABLE_NAME " SET "
                NOTE_SQL_COL02 " = (:_v02),"
                NOTE_SQL_COL03 " = (:_v03),"
                NOTE_SQL_COL04 " = (:_v04),"
                NOTE_SQL_COL05 " = (:_v05),"
                NOTE_SQL_COL06 " = (:_v06),"
                NOTE_SQL_COL07 " = (:_v07) "
                "WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v02", m_date);
  query.bindValue(":_v03", m_supplier.m_id.getIdNull());
  query.bindValue(":_v04", m_observation);
  query.bindValue(":_v05", m_disccount);
  query.bindValue(":_v06", m_employee.m_id.getIdNull());
  query.bindValue(":_v07", (int)m_paymentMethod);
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    bSuccess = PurchaseElement::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_vItem.size() && bSuccess; ++i)
    {
      m_vItem[i].m_ownerId = m_id;
      bSuccess = m_vItem.at(i).SQL_insert_proc(query);
    }
  }

  if (bSuccess)
  {
    query.prepare("DELETE FROM " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME " WHERE " NOTE_PAYMENT_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vPaymentItem.size() && bSuccess; ++i)
    {
      query.prepare("INSERT INTO " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME " ("
                    NOTE_PAYMENT_ITEMS_SQL_COL01 ","
                    NOTE_PAYMENT_ITEMS_SQL_COL02 ","
                    NOTE_PAYMENT_ITEMS_SQL_COL03
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vPaymentItem.at(i).m_date);
      query.bindValue(":_v03", m_vPaymentItem.at(i).m_value);
      bSuccess = query.exec();
      if (bSuccess)
        m_vPaymentItem.at(i).m_id.set(query.lastInsertId().toLongLong());
    }
  }

  return bSuccess;
}

bool Purchase::SQL_select_proc(QSqlQuery& query, QString& error)
{
  clear(false);
  error.clear();
  query.prepare("SELECT "
                NOTE_SQL_COL01 ","
                NOTE_SQL_COL02 ","
                NOTE_SQL_COL03 ","
                NOTE_SQL_COL04 ","
                NOTE_SQL_COL05 ","
                NOTE_SQL_COL06 ","
                NOTE_SQL_COL07
                " FROM " NOTE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_number = query.value(0).toLongLong();
      m_date = query.value(1).toDate();
      m_supplier.m_id.set(query.value(2).toLongLong());
      m_observation = query.value(3).toString();
      m_disccount = query.value(4).toDouble();
      m_employee.m_id.set(query.value(5).toDouble());
      m_paymentMethod = (PaymentMethod)query.value(6).toInt();
    }
    else
    {
      error = "Vale não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = PurchaseElement::SQL_select_by_owner_id_proc(query, m_id, m_vItem, error);


  if (bSuccess)
  {
    if (m_supplier.m_id.isValid())
      bSuccess = m_supplier.SQL_select_proc(query, error);
  }

  if (bSuccess && m_employee.m_id.isValid())
    m_employee.SQL_select_proc(query, error);

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID ","
                  NOTE_PAYMENT_ITEMS_SQL_COL02 ","
                  NOTE_PAYMENT_ITEMS_SQL_COL03
                  " FROM " NOTE_PAYMENT_ITEMS_SQL_TABLE_NAME
                  " WHERE " NOTE_PAYMENT_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      while (query.next())
      {
        PaymentItem o;
        o.m_id.set(query.value(0).toLongLong());
        o.m_date = query.value(1).toDate();
        o.m_value = query.value(2).toDouble();
        m_vPaymentItem.push_back(o);
      }
    }
  }

  return bSuccess;
}

bool Purchase::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " NOTE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

void Purchase::setEmployee(const JItemSQL& e) const
{
  m_employee = dynamic_cast<const Employee&>(e);
}
