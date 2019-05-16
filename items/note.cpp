#include "note.h"
#include "jmodel.h"

class NoteModel : public JModel
{
public:
  NoteModel(QObject *parent)
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

NoteItem::NoteItem()
{
  clear();
}

void NoteItem::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_product.clear();
  m_ammount = 0.0;
  m_price = 0.0;
  m_package.clear();
}

bool NoteItem::isValid() const
{
  return true;
}

bool NoteItem::operator !=(const JItem& other) const
{
  const NoteItem& another = dynamic_cast<const NoteItem&>(other);
  return
      m_product.m_id != another.m_product.m_id ||
      m_ammount != another.m_ammount ||
      m_price != another.m_price ||
      m_package != another.m_package;
}

bool NoteItem::operator ==(const JItem& other) const
{
  return !(*this != other);
}

QString NoteItem::strTableName() const
{
  return NOTE_ITEMS_SQL_TABLE_NAME;
}

double NoteItem::subtotal() const
{
  return m_ammount * m_price;
}

QString NoteItem::strSubtotal() const
{
  return st_strMoney(subtotal());
}

QString NoteItem::strAmmount() const
{
  return st_strAmmount(m_ammount);
}

QString NoteItem::strPrice() const
{
  return st_strMoney(m_price);
}

Note::Note(Id id)
{
  clear();
  m_id = id;
}

void Note::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_payment.clear(bClearId);
  m_number = 0;
  m_supplier.clear();
  m_date = QDate::currentDate();
  m_vNoteItem.clear();
  m_observation.clear();
  m_disccount = 0.0;
  m_employee.clear();
}

bool Note::isValid() const
{
  return true;
}

bool Note::operator !=(const JItem& other) const
{
  const Note& another = dynamic_cast<const Note&>(other);
  return
      m_number != another.m_number ||
      m_date != another.m_date ||
      m_supplier.m_id != another.m_supplier.m_id ||
      m_payment != another.m_payment ||
      m_vNoteItem != another.m_vNoteItem ||
      m_disccount != another.m_disccount;
}

bool Note::operator ==(const JItem& other) const
{
  return !(*this != other);
}

QString Note::strDate() const
{
  return m_date.toString("dd/MM/yyyy");
}

QString Note::strDayOfWeek() const
{
  return m_date.toString("dddd");
}

QString Note::strNumber() const
{
  return st_strInt(m_number);
}

QString Note::strDisccount() const
{
  return st_strMoney(m_disccount);
}

double Note::total() const
{
  double total = 0.0;
  for (int i = 0; i != m_vNoteItem.size(); ++i)
    total += m_vNoteItem.at(i).subtotal();
  return total + m_disccount;
}

double Note::subTotal() const
{
  double subTotal = 0.0;
  for (int i = 0; i != m_vNoteItem.size(); ++i)
    subTotal += m_vNoteItem.at(i).subtotal();
  return subTotal;
}

QString Note::strTotal() const
{
  return st_strMoney(total());
}

QString Note::strSubTotal() const
{
  return st_strMoney(subTotal());
}

bool Note::isPaymentOk() const
{
  return total() == m_payment.total();
}

QString Note::SQL_tableName() const
{
  return NOTE_SQL_TABLE_NAME;
}

bool Note::SQL_insert_proc(QSqlQuery& query) const
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
                  NOTE_SQL_COL06
                  ") VALUES ("
                  "(:_v01),"
                  "(:_v02),"
                  "(:_v03),"
                  "(:_v04),"
                  "(:_v05),"
                  "(:_v06))");
    query.bindValue(":_v01", m_number);
    query.bindValue(":_v02", m_date);
    query.bindValue(":_v03", m_supplier.m_id.getIdNull());
    query.bindValue(":_v04", m_observation);
    query.bindValue(":_v05", m_disccount);
    query.bindValue(":_v06", m_employee.m_id.getIdNull());
    bSuccess = query.exec();
  }

  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vNoteItem.size(); ++i)
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
      query.bindValue(":_v02", m_vNoteItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_vNoteItem.at(i).m_ammount);
      query.bindValue(":_v04", m_vNoteItem.at(i).m_price);
      query.bindValue(":_v05", m_vNoteItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", m_vNoteItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", m_vNoteItem.at(i).m_package.m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        m_vNoteItem.at(i).m_id.set(query.lastInsertId().toLongLong());
      else
        break;
    }
  }

  if (bSuccess)
  {
    m_payment.m_noteId = m_id;
    bSuccess = m_payment.SQL_insert_proc(query);
  }

  return bSuccess;
}

bool Note::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " NOTE_SQL_TABLE_NAME " SET "
                NOTE_SQL_COL02 " = (:_v02),"
                NOTE_SQL_COL03 " = (:_v03),"
                NOTE_SQL_COL04 " = (:_v04),"
                NOTE_SQL_COL05 " = (:_v05),"
                NOTE_SQL_COL06 " = (:_v06) "
                "WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v02", m_date);
  query.bindValue(":_v03", m_supplier.m_id.getIdNull());
  query.bindValue(":_v04", m_observation);
  query.bindValue(":_v05", m_disccount);
  query.bindValue(":_v06", m_employee.m_id.getIdNull());
  bool bSuccess = query.exec();

  query.prepare("DELETE FROM " NOTE_ITEMS_SQL_TABLE_NAME " WHERE " NOTE_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", m_id.get());
  bSuccess = query.exec();

  if (bSuccess)
  {
    for (int i = 0; i != m_vNoteItem.size() && bSuccess; ++i)
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
      query.bindValue(":_v02", m_vNoteItem.at(i).m_product.m_id.get());
      query.bindValue(":_v03", m_vNoteItem.at(i).m_ammount);
      query.bindValue(":_v04", m_vNoteItem.at(i).m_price);
      query.bindValue(":_v05", m_vNoteItem.at(i).m_package.m_bIsPackage);
      query.bindValue(":_v06", m_vNoteItem.at(i).m_package.m_unity);
      query.bindValue(":_v07", m_vNoteItem.at(i).m_package.m_ammount);
      bSuccess = query.exec();
      if (bSuccess)
        m_vNoteItem.at(i).m_id.set(query.lastInsertId().toLongLong());
    }
  }

  if (bSuccess)
  {
    if (m_payment.m_id.isValid())
      m_payment.SQL_update_proc(query);
    else
      m_payment.SQL_insert_proc(query);
  }

  return bSuccess;
}

bool Note::SQL_select_proc(QSqlQuery& query, QString& error)
{
  clear(false);
  error.clear();
  query.prepare("SELECT "
                NOTE_SQL_COL01 ","
                NOTE_SQL_COL02 ","
                NOTE_SQL_COL03 ","
                NOTE_SQL_COL04 ","
                NOTE_SQL_COL05 ","
                NOTE_SQL_COL06
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
        NoteItem o;
        o.m_id.set(query.value(0).toLongLong());
        o.m_product.m_id.set(query.value(1).toLongLong());
        o.m_ammount = query.value(2).toDouble();
        o.m_price = query.value(3).toDouble();
        o.m_package.m_bIsPackage = query.value(4).toBool();
        o.m_package.m_unity = query.value(5).toString();
        o.m_package.m_ammount = query.value(6).toDouble();
        m_vNoteItem.push_back(o);
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
    for (int i = 0; i != m_vNoteItem.size(); ++i)
    {
      if (m_vNoteItem.at(i).m_product.m_id.isValid())
        bSuccess = m_vNoteItem[i].m_product.SQL_select_proc(query, error);
      if (!bSuccess)
        break;
    }
  }

  if (bSuccess)
  {
    QString error2;
    m_payment.m_noteId = m_id;
    m_payment.SQL_select_proc_by_noteid(query, error2);
  }

  return bSuccess;
}

bool Note::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " NOTE_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

NoteItem Note::SQL_select_last_item(Id supplierId, Id productId)
{
  NoteItem o;
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

JModel* Note::SQL_table_model(QObject* parent) const
{
  return new NoteModel(parent);
}

void Note::setEmployee(const JItemSQL& e) const
{
  m_employee = dynamic_cast<const Employee&>(e);
}
