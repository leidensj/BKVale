#include "purchase.h"
#include "jmodel.h"

class PurchaseModel : public JModel
{
public:
  PurchaseModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     NOTE_SQL_TABLE_NAME "." SQL_COLID ","
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL01 ","
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL02 ","
                     FORM_SQL_TABLE_NAME "." FORM_SQL_COL03 ","
                     FORM_SQL_TABLE_NAME "." FORM_SQL_COL02 ","
                     "(COALESCE(_TTOTAL._TSUBTOTAL,0) + " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL05 ")"
                     " FROM " NOTE_SQL_TABLE_NAME
                     " LEFT OUTER JOIN "
                     "(SELECT " NOTE_ITEMS_SQL_COL01 ","
                     "COALESCE(SUM(" NOTE_ITEMS_SQL_COL03 "*" NOTE_ITEMS_SQL_COL04 "), 0) AS _TSUBTOTAL"
                     " FROM " NOTE_ITEMS_SQL_TABLE_NAME
                     " GROUP BY " NOTE_ITEMS_SQL_COL01 ") AS _TTOTAL"
                     " ON " NOTE_SQL_TABLE_NAME "." SQL_COLID "= _TTOTAL." NOTE_ITEMS_SQL_COL01
                     " LEFT OUTER JOIN "
                     SUPPLIER_SQL_TABLE_NAME
                     " ON " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03 "=" SUPPLIER_SQL_TABLE_NAME "." SQL_COLID
                     " LEFT OUTER JOIN "
                     FORM_SQL_TABLE_NAME
                     " ON " SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL01 " = " FORM_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  void select(QHeaderView* header)
  {
    JModel::select(getStrQuery());
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Número"));
    setHeaderData(2, Qt::Horizontal, tr("Data"));
    setHeaderData(3, Qt::Horizontal, tr("Fornecedor"));
    setHeaderData(4, Qt::Horizontal, tr("Fornecedor"));
    setHeaderData(5, Qt::Horizontal, tr("Total"));
    if (header != nullptr)
    {
      if (header->count() == 6)
      {
        header->hideSection(0);
        header->hideSection(4);
        header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
        header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
        header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
        header->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
      }
    }
  }

  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
  {
    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DisplayRole)
    {
      if (idx.column() == 2)
        value = QDate::fromString(value.toString(), Qt::ISODate).toString("yyyy/MM/dd");
      else if (idx.column() == 3)
      {
        if (value.toString().isEmpty())
          value = QSqlQueryModel::data(idx.sibling(idx.row(), idx.column() + 1), role);
      }
      else if (idx.column() == 5)
        value = "R$ " + QString::number(value.toDouble(), 'f', 2);
    }
    return value;
  }
};

PurchaseItem::PurchaseItem()
{
  clear();
}

void PurchaseItem::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
  m_price = 0.0;
  m_package.clear();
}

bool PurchaseItem::isValid() const
{
  return true;
}

bool PurchaseItem::operator !=(const JItem& other) const
{
  const PurchaseItem& another = dynamic_cast<const PurchaseItem&>(other);
  return
      m_product.m_id != another.m_product.m_id ||
      m_ammount != another.m_ammount ||
      m_price != another.m_price ||
      m_package != another.m_package;
}

bool PurchaseItem::operator ==(const JItem& other) const
{
  return !(*this != other);
}

double PurchaseItem::subtotal() const
{
  return m_ammount * m_price;
}

QString PurchaseItem::strSubtotal() const
{
  return st_strMoney(subtotal());
}

QString PurchaseItem::strAmmount() const
{
  return st_strAmmount(m_ammount);
}

QString PurchaseItem::strPrice() const
{
  return st_strMoney(m_price);
}

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
      query.prepare("INSERT INTO " NOTE_ITEMS_SQL_TABLE_NAME " ("
                    NOTE_ITEMS_SQL_COL01 ","
                    NOTE_ITEMS_SQL_COL02 ","
                    NOTE_ITEMS_SQL_COL03 ","
                    NOTE_ITEMS_SQL_COL04 ","
                    NOTE_ITEMS_SQL_COL05 ","
                    NOTE_ITEMS_SQL_COL06 ","
                    NOTE_ITEMS_SQL_COL07
                    ") VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05),"
                    "(:_v06),"
                    "(:_v07))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_vItem.at(i).m_ammount);
      query.bindValue(":_v04", m_vItem.at(i).m_price);
      query.bindValue(":_v05", m_vItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", m_vItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", m_vItem.at(i).m_package.m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        m_vItem.at(i).m_id.set(query.lastInsertId().toLongLong());
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
    query.prepare("DELETE FROM " NOTE_ITEMS_SQL_TABLE_NAME " WHERE " NOTE_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    for (int i = 0; i != m_vItem.size() && bSuccess; ++i)
    {
      query.prepare("INSERT INTO " NOTE_ITEMS_SQL_TABLE_NAME " ("
                    NOTE_ITEMS_SQL_COL01 ","
                    NOTE_ITEMS_SQL_COL02 ","
                    NOTE_ITEMS_SQL_COL03 ","
                    NOTE_ITEMS_SQL_COL04 ","
                    NOTE_ITEMS_SQL_COL05 ","
                    NOTE_ITEMS_SQL_COL06 ","
                    NOTE_ITEMS_SQL_COL07
                    " ) VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03),"
                    "(:_v04),"
                    "(:_v05),"
                    "(:_v06),"
                    "(:_v07))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_vItem.at(i).m_ammount);
      query.bindValue(":_v04", m_vItem.at(i).m_price);
      query.bindValue(":_v05", m_vItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", m_vItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", m_vItem.at(i).m_package.m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        m_vItem.at(i).m_id.set(query.lastInsertId().toLongLong());
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
  {
    query.prepare("SELECT "
                  SQL_COLID ","
                  NOTE_ITEMS_SQL_COL02 ","
                  NOTE_ITEMS_SQL_COL03 ","
                  NOTE_ITEMS_SQL_COL04 ","
                  NOTE_ITEMS_SQL_COL05 ","
                  NOTE_ITEMS_SQL_COL06 ","
                  NOTE_ITEMS_SQL_COL07
                  " FROM " NOTE_ITEMS_SQL_TABLE_NAME
                  " WHERE " NOTE_ITEMS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      while (query.next())
      {
        PurchaseItem o;
        o.m_id.set(query.value(0).toLongLong());
        o.m_product.m_id.set(query.value(1).toLongLong());
        o.m_ammount = query.value(2).toDouble();
        o.m_price = query.value(3).toDouble();
        o.m_package.m_bIsPackage = query.value(4).toBool();
        o.m_package.m_unity = query.value(5).toString();
        o.m_package.m_ammount = query.value(6).toDouble();
        m_vItem.push_back(o);
      }
    }
  }

  if (bSuccess)
  {
    if (m_supplier.m_id.isValid())
      bSuccess = m_supplier.SQL_select_proc(query, error);
  }

  if (bSuccess && m_employee.m_id.isValid())
    m_employee.SQL_select_proc(query, error);

  if (bSuccess)
  {
    for (int i = 0; i != m_vItem.size(); ++i)
    {
      if (m_vItem.at(i).m_product.m_id.isValid())
        bSuccess = m_vItem[i].m_product.SQL_select_proc(query, error);
      if (!bSuccess)
        break;
    }
  }

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

PurchaseItem Purchase::SQL_select_last_item(Id supplierId, Id productId)
{
  PurchaseItem o;
  QString error;
  if (!SQL_isOpen(error))
    return o;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  query.prepare("SELECT "
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL01 ","
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL02 ","
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL03 ","
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL04 ","
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL05 ","
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL06 ","
                NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL07
                " FROM " NOTE_SQL_TABLE_NAME
                " INNER JOIN " NOTE_ITEMS_SQL_TABLE_NAME
                " ON " NOTE_SQL_TABLE_NAME "." SQL_COLID
                " = " NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL01
                " WHERE " NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03
                " = (:_v01)"
                " AND " NOTE_ITEMS_SQL_TABLE_NAME "." NOTE_ITEMS_SQL_COL02
                " = (:_v02) "
                " ORDER BY " NOTE_ITEMS_SQL_TABLE_NAME "." SQL_COLID
                " DESC LIMIT 1");
  query.bindValue(":_v01", supplierId.get());
  query.bindValue(":_v02", productId.get());
  bool bSuccess = query.exec();

  if (bSuccess && query.next())
  {
    o.m_id.set(query.value(0).toLongLong());
    o.m_product.m_id.set(query.value(1).toLongLong());
    o.m_ammount = query.value(2).toDouble();
    o.m_price = query.value(3).toDouble();
    o.m_package.m_bIsPackage = query.value(4).toBool();
    o.m_package.m_unity = query.value(5).toString();
    o.m_package.m_ammount = query.value(6).toDouble();
    if (o.m_product.m_id.isValid())
      bSuccess = o.m_product.SQL_select_proc(query, error);
  }

  SQL_finish(db, query, bSuccess, error);
  return o;
}

JModel* Purchase::SQL_table_model(QObject* parent) const
{
  return new PurchaseModel(parent);
}

void Purchase::setEmployee(const JItemSQL& e) const
{
  m_employee = dynamic_cast<const Employee&>(e);
}
