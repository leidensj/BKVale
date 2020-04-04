#include "purchase.h"

QIcon Purchase::st_paymentIcon(PaymentMethod e)
{
  switch (e)
  {
    case PaymentMethod::Credit:
      return QIcon(":/icons/res/credit.png");
    case PaymentMethod::Cash:
      return QIcon(":/icons/res/cash.png");
    case PaymentMethod::Bonus:
      return QIcon(":/icons/res/bonus.png");
    default:
      return QIcon();
  }
}

QString Purchase::st_paymentText(PaymentMethod e)
{
  switch (e)
  {
    case PaymentMethod::Credit:
      return QObject::tr("A prazo");
    case PaymentMethod::Cash:
      return QObject::tr("A vista");
    case PaymentMethod::Bonus:
      return QObject::tr("Bonus");
    default:
      return "";
  }
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
  m_vPayment.clear();
  m_number = 0;
  m_supplier.clear();
  m_date = QDate::currentDate();
  m_vElement.clear();
  m_observation.clear();
  m_disccount = 0.0;
  m_employee.clear();
  m_store.clear();
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
      m_vPayment != another.m_vPayment ||
      m_vElement != another.m_vElement ||
      m_disccount != another.m_disccount ||
      m_store != another.m_store;
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
  return Data::strInt(m_number);
}

QString Purchase::strDisccount() const
{
  return Data::strMoney(m_disccount);
}

double Purchase::total() const
{
  double total = 0.0;
  for (int i = 0; i != m_vElement.size(); ++i)
    total += m_vElement.at(i).subtotal();
  return total + m_disccount;
}

double Purchase::subTotal() const
{
  double subTotal = 0.0;
  for (int i = 0; i != m_vElement.size(); ++i)
    subTotal += m_vElement.at(i).subtotal();
  return subTotal;
}

QString Purchase::strTotal() const
{
  return Data::strMoney(total());
}

QString Purchase::strSubTotal() const
{
  return Data::strMoney(subTotal());
}

bool Purchase::isPaymentValid() const
{
  if (m_paymentMethod == PaymentMethod::Credit)
  {
    bool bValid = true;
    for (int i = 0; i != m_vPayment.size() && bValid; ++i)
      bValid = m_vPayment.at(i).m_date >= m_date;
    return bValid && Data::areEqual(total(), paymentTotal(), Data::Type::Money);
  }
  return true;
}

double Purchase::paymentTotal() const
{
  if (m_paymentMethod == PaymentMethod::Credit)
  {
    double t = 0.0;
    for (int i = 0; i != m_vPayment.size(); ++i)
      t += m_vPayment.at(i).m_value;
    return t;
  }
  return total();
}

void Purchase::adjustPayment()
{
  if (!isPaymentValid())
  {
    m_vPayment.clear();
    m_paymentMethod = PaymentMethod::Credit;
    PaymentElement o;
    o.m_value = total();
    o.m_date = m_date.addMonths(1);
    m_vPayment.push_back(o);
  }
}

QString Purchase::SQL_tableName() const
{
  return PURCHASE_SQL_TABLE_NAME;
}

bool Purchase::SQL_insert_proc(QSqlQuery& query) const
{
  bool bSuccess = query.exec("SELECT MAX(" PURCHASE_SQL_COL_NMB ") FROM " PURCHASE_SQL_TABLE_NAME);
  if (bSuccess)
  {
    m_number = query.next()
               ? query.value(0).toLongLong() + 1
               : PURCHASE_DEFAULT_NUMBER;
    m_number = m_number > PURCHASE_DEFAULT_NUMBER ? m_number : PURCHASE_DEFAULT_NUMBER;

    query.prepare("INSERT INTO " PURCHASE_SQL_TABLE_NAME " ("
                  PURCHASE_SQL_COL_NMB ","
                  PURCHASE_SQL_COL_DTE ","
                  PURCHASE_SQL_COL_SPL ","
                  PURCHASE_SQL_COL_OBS ","
                  PURCHASE_SQL_COL_DSC ","
                  PURCHASE_SQL_COL_EMP ","
                  PURCHASE_SQL_COL_MTH ","
                  PURCHASE_SQL_COL_STR
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04),"
                  "(:_v05),"
                  "(:_v06),"
                  "(:_v07),"
                  "(:_v08))");
    query.bindValue(":_v01", m_number);
    query.bindValue(":_v02", m_date);
    query.bindValue(":_v03", m_supplier.m_id.getIdNull());
    query.bindValue(":_v04", m_observation);
    query.bindValue(":_v05", m_disccount);
    query.bindValue(":_v06", m_employee.m_id.getIdNull());
    query.bindValue(":_v07", (int)m_paymentMethod);
    query.bindValue(":_v08", m_store.m_id.getIdNull());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vElement.size() && bSuccess; ++i)
    {
      m_vElement[i].m_ownerId = m_id;
      bSuccess = m_vElement.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_vPayment.size() && bSuccess; ++i)
    {
      m_vPayment[i].m_ownerId = m_id;
      bSuccess = m_vPayment.at(i).SQL_insert_proc(query);
    }
  }

  return bSuccess;
}

bool Purchase::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " PURCHASE_SQL_TABLE_NAME " SET "
                PURCHASE_SQL_COL_DTE " = (:_v02),"
                PURCHASE_SQL_COL_SPL " = (:_v03),"
                PURCHASE_SQL_COL_OBS " = (:_v04),"
                PURCHASE_SQL_COL_DSC " = (:_v05),"
                PURCHASE_SQL_COL_EMP " = (:_v06),"
                PURCHASE_SQL_COL_MTH " = (:_v07),"
                PURCHASE_SQL_COL_STR " = (:_v08) "
                "WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v02", m_date);
  query.bindValue(":_v03", m_supplier.m_id.getIdNull());
  query.bindValue(":_v04", m_observation);
  query.bindValue(":_v05", m_disccount);
  query.bindValue(":_v06", m_employee.m_id.getIdNull());
  query.bindValue(":_v07", (int)m_paymentMethod);
  query.bindValue(":_v08", m_store.m_id.getIdNull());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    bSuccess = PurchaseElement::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_vElement.size() && bSuccess; ++i)
    {
      m_vElement[i].m_ownerId = m_id;
      bSuccess = m_vElement.at(i).SQL_insert_proc(query);
    }
  }

  if (bSuccess)
  {
    bSuccess = PaymentElement::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_vPayment.size(); ++i)
    {
      m_vPayment[i].m_ownerId = m_id;
      bSuccess = m_vPayment.at(i).SQL_insert_proc(query);
    }
  }

  return bSuccess;
}

bool Purchase::SQL_select_proc(QSqlQuery& query, QString& error)
{
  clear(false);
  error.clear();
  query.prepare("SELECT "
                PURCHASE_SQL_COL_NMB ","
                PURCHASE_SQL_COL_DTE ","
                PURCHASE_SQL_COL_SPL ","
                PURCHASE_SQL_COL_OBS ","
                PURCHASE_SQL_COL_DSC ","
                PURCHASE_SQL_COL_EMP ","
                PURCHASE_SQL_COL_MTH ","
                PURCHASE_SQL_COL_STR
                " FROM " PURCHASE_SQL_TABLE_NAME
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
      m_store.m_id.set(query.value(7).toLongLong());
    }
    else
    {
      error = "Vale não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = PurchaseElement::SQL_select_by_owner_id_proc(query, m_id, m_vElement, error);

  if (bSuccess)
    bSuccess = PaymentElement::SQL_select_by_owner_id_proc(query, m_id, m_vPayment, error);

  if (bSuccess && m_supplier.m_id.isValid())
    bSuccess = m_supplier.SQL_select_proc(query, error);

  if (bSuccess && m_employee.m_id.isValid())
    bSuccess = m_employee.SQL_select_proc(query, error);

  if (bSuccess && m_store.m_id.isValid())
    bSuccess = m_store.SQL_select_proc(query, error);

  return bSuccess;
}

bool Purchase::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " PURCHASE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

void Purchase::setEmployee(const JItemSQL& e) const
{
  m_employee = dynamic_cast<const Employee&>(e);
}

bool Purchase::SQL_select_all_supplier_id_items()
{
  QString error;
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  error.clear();
  query.prepare("SELECT DISTINCT "
                PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID
                " FROM " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                " LEFT OUTER JOIN " PURCHASE_SQL_TABLE_NAME " ON "
                PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_NID
                " = " PURCHASE_SQL_TABLE_NAME "." SQL_COLID " WHERE "
                PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SPL " = (:_v00)");
  query.bindValue(":_v00", m_supplier.m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    m_vElement.clear();
    while (query.next())
    {
      PurchaseElement e;
      e.SQL_select_last(m_supplier.m_id, Id(query.value(0).toLongLong()));
      m_vElement.push_back(e);
    }
  }

  return SQL_finish(db, query, bSuccess, error);
}

QVector<Id> Purchase::st_SQL_select_all_purchases(const QString& filter)
{
  QVector<Id> ids;
  QString error;
  if (!SQL_isOpen(error))
    return ids;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);
  QString str;
  str = "SELECT DISTINCT " SQL_COLID " FROM " PURCHASE_SQL_TABLE_NAME;
  if (!filter.isEmpty())
    str += " WHERE " + filter;
  query.prepare(str);
  bool bSuccess = query.exec();
  while (bSuccess && query.next())
    ids.push_back(query.value(0).toLongLong());
  SQL_finish(db, query, bSuccess, error);
  return ids;
}
