#include "purchase.h"
#include "escpos.h"
#include "QHostInfo"

//todo tirar o qhostinfo e passar nos args

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
      return QObject::tr("Bonificação");
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
  m_payments.clear();
  m_number = 0;
  m_supplier.clear();
  m_date = QDate::currentDate();
  m_products.clear();
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
      m_payments != another.m_payments ||
      m_products != another.m_products ||
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
  for (int i = 0; i != m_products.size(); ++i)
    total += m_products.at(i).subtotal();
  return total + m_disccount;
}

double Purchase::subTotal() const
{
  double subTotal = 0.0;
  for (int i = 0; i != m_products.size(); ++i)
    subTotal += m_products.at(i).subtotal();
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
    for (int i = 0; i != m_payments.size() && bValid; ++i)
      bValid = m_payments.at(i).m_date >= m_date;
    return bValid && Data::areEqual(total(), paymentTotal(), Data::Type::Money);
  }
  return true;
}

double Purchase::paymentTotal() const
{
  if (m_paymentMethod == PaymentMethod::Credit)
  {
    double t = 0.0;
    for (int i = 0; i != m_payments.size(); ++i)
      t += m_payments.at(i).m_value;
    return t;
  }
  return total();
}

void Purchase::adjustPayment()
{
  if (!isPaymentValid())
  {
    m_payments.clear();
    m_paymentMethod = PaymentMethod::Credit;
    PaymentPart o;
    o.m_value = total();
    o.m_date = m_date.addMonths(1);
    m_payments.push_back(o);
  }
}

QString Purchase::SQL_tableName() const
{
  return PURCHASE_SQL_TABLE_NAME;
}

bool Purchase::SQL_insert_proc(QSqlQuery& query) const
{
  bool bSuccess = query.exec("SELECT MAX(" PURCHASE_SQL_COL_NUM ") FROM " PURCHASE_SQL_TABLE_NAME);
  if (bSuccess)
  {
    m_number = query.next()
               ? query.value(0).toLongLong() + 1
               : PURCHASE_DEFAULT_NUMBER;
    m_number = m_number > PURCHASE_DEFAULT_NUMBER ? m_number : PURCHASE_DEFAULT_NUMBER;

    query.prepare("INSERT INTO " PURCHASE_SQL_TABLE_NAME " ("
                  PURCHASE_SQL_COL_NUM ","
                  PURCHASE_SQL_COL_DAT ","
                  PURCHASE_SQL_COL_SID ","
                  PURCHASE_SQL_COL_OBS ","
                  PURCHASE_SQL_COL_DIS ","
                  PURCHASE_SQL_COL_EID ","
                  PURCHASE_SQL_COL_PAY ","
                  PURCHASE_SQL_COL_TID
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
    for (int i = 0; i != m_products.size() && bSuccess; ++i)
    {
      m_products[i].m_ownerId = m_id;
      bSuccess = m_products.at(i).SQL_insert_proc(query);
    }
    for (int i = 0; i != m_payments.size() && bSuccess; ++i)
    {
      m_payments[i].m_ownerId = m_id;
      bSuccess = m_payments.at(i).SQL_insert_proc(query);
    }
  }

  return bSuccess;
}

bool Purchase::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " PURCHASE_SQL_TABLE_NAME " SET "
                PURCHASE_SQL_COL_DAT " = (:_v02),"
                PURCHASE_SQL_COL_SID " = (:_v03),"
                PURCHASE_SQL_COL_OBS " = (:_v04),"
                PURCHASE_SQL_COL_DIS " = (:_v05),"
                PURCHASE_SQL_COL_EID " = (:_v06),"
                PURCHASE_SQL_COL_PAY " = (:_v07),"
                PURCHASE_SQL_COL_TID " = (:_v08) "
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
    bSuccess = PurchaseProduct::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_products.size() && bSuccess; ++i)
    {
      m_products[i].m_ownerId = m_id;
      bSuccess = m_products.at(i).SQL_insert_proc(query);
    }
  }

  if (bSuccess)
  {
    bSuccess = PaymentPart::SQL_remove_by_owner_id_proc(query, m_id);
    for (int i = 0; i != m_payments.size(); ++i)
    {
      m_payments[i].m_ownerId = m_id;
      bSuccess = m_payments.at(i).SQL_insert_proc(query);
    }
  }

  return bSuccess;
}

bool Purchase::SQL_select_proc(QSqlQuery& query, QString& error)
{
  clear(false);
  error.clear();
  query.prepare("SELECT "
                PURCHASE_SQL_COL_NUM ","
                PURCHASE_SQL_COL_DAT ","
                PURCHASE_SQL_COL_SID ","
                PURCHASE_SQL_COL_OBS ","
                PURCHASE_SQL_COL_DIS ","
                PURCHASE_SQL_COL_EID ","
                PURCHASE_SQL_COL_PAY ","
                PURCHASE_SQL_COL_TID
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
    bSuccess = PurchaseProduct::SQL_select_by_owner_id_proc(query, m_id, m_products, error);

  if (bSuccess)
    bSuccess = PaymentPart::SQL_select_by_owner_id_proc(query, m_id, m_payments, error);

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
                PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SID " = (:_v00)");
  query.bindValue(":_v00", m_supplier.m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    m_products.clear();
    while (query.next())
    {
      PurchaseProduct o;
      o.SQL_select_last(m_supplier.m_id, Id(query.value(0).toLongLong()));
      m_products.push_back(o);
    }
  }

  return SQL_finish(db, query, bSuccess, error);
}

bool Purchase::SQL_last_purchase_that_contains_productid(Id pId)
{
  QString error;
  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  error.clear();
  query.prepare("SELECT "
                PURCHASE_SQL_TABLE_NAME "." SQL_COLID
                " FROM " PURCHASE_SQL_TABLE_NAME
                " LEFT JOIN " PURCHASE_ELEMENTS_SQL_TABLE_NAME " ON "
                PURCHASE_SQL_TABLE_NAME "." SQL_COLID
                " = " PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_NID " WHERE "
                PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID " = (:_v00) "
                "ORDER BY " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DAT " DESC LIMIT 1");
  query.bindValue(":_v00", pId.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_id.set(query.value(0).toLongLong());
      bSuccess = SQL_select_proc(query, error);
    }
  }

  return SQL_finish(db, query, bSuccess, error);
}

PurchaseProduct Purchase::findProduct(Id pId) const
{
  PurchaseProduct o;
  for (int i = 0; i != m_products.size(); ++i)
  {
    if (pId == m_products.at(i).m_product.m_id)
    {
      o = m_products.at(i);
      break;
    }
  }
  return o;
}

QByteArray Purchase::printVersion(const QVariant& /*arg*/) const
{
  EscPos ep;
  if (m_store.m_id.isValid())
  {
    ep.align(true);
    ep.expand(true);
    ep.str(m_store.m_form.m_alias + "\n");
    ep.expand(false);
    ep.str(m_store.m_form.m_name + "\n");
    if (!m_store.m_form.m_vAddress.isEmpty())
      ep.str(m_store.m_form.m_vAddress.at(0).name() + "\n");
    if (!m_store.m_form.m_vPhone.isEmpty())
      ep.str(m_store.m_form.m_vPhone.at(0).name() + "\n");
    ep.str("\n");
  }
  ep.str(m_paymentMethod == Purchase::PaymentMethod::Cash ?
        "PAGAMENTO A VISTA\n\n" :
        "ORDEM DE RECEBIMENTO\nDE MERCADORIA\n\n");
  ep.doublefont(true);
  ep.str(strNumber() + "\n\n");
  ep.doublefont(false);
  ep.str("Data ");
  ep.doublefont(true);
  ep.str(strDate());
  ep.doublefont(false);
  ep.str(" " + strDayOfWeek() + "\n");
  ep.doublefont(true);
  ep.str(m_supplier.m_id.isValid() ? m_supplier.m_form.strAliasName() : "Nao informado");
  ep.doublefont(false);
  ep.str("\nFornecedor\n\n");
  ep.align();
  for (const auto& _o :  m_products)
  {
    ep.str(_o.m_product.m_name + "\n");
    {
      QString itemPt1 = _o.strAmmount();
      itemPt1 += _o.m_package.strUnity(_o.m_product.m_unity);
      itemPt1 += " x " + _o.strPrice();
      QString itemPt2 = _o.strSubtotal();
      const int n = TABLE_WIDTH - (itemPt1.length() + itemPt2.length());
      for (int j = 0; j < n; ++j)
        itemPt1 += ".";
      ep.str(itemPt1);
      ep.bold(true);
      ep.str(itemPt2 + "\n");
      ep.bold(false);
    }
  }
  ep.str("\n");

  if (m_disccount != 0)
  {
    ep.expand(true);
    ep.str("Subtotal:  " + strSubTotal() + "\n");
    if (m_disccount > 0)
      ep.str("Acrescimo: " + strDisccount() + "\n\n");
    else if(m_disccount < 0)
      ep.str("Desconto:  " + strDisccount() + "\n\n");
    ep.expand(false);
  }

  QDateTime dt = DateTime::server().toLocalTime();

  if (!m_observation.isEmpty())
  {
    ep.expand(true);
    ep.str("Observacoes: ");
    ep.expand(false);
    ep.str(m_observation + "\n\n");
  }

  ep.align(true);
  ep.doublefont(true);
  ep.str("TOTAL " + strTotal());
  ep.doublefont(false);
  ep.str("\nEmissao: " + dt.date().toString("dd/MM/yyyy ") + dt.time().toString("hh:mm:ss") + " @ " + QHostInfo::localHostName().toUpper() + "\n\n\n");
  ep.str("________________________________\nAssinatura " + m_employee.m_form.strAliasName() + "\n");

  if (m_paymentMethod == Purchase::PaymentMethod::Cash)
  {
    ep.str("\n\n________________________________\nAssinatura ");
    if (m_supplier.m_id.isValid())
      ep.str(m_supplier.m_form.strAliasName());
    else
      ep.str("fornecedor");
  }
  ep.str("\n\n");
  ep.cut();
  return ep.m_ba;
}
