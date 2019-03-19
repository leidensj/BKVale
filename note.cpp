#include "note.h"


NoteItem::NoteItem()
{
  clear();
}

void NoteItem::clear()
{
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

Note::Note()
{
  clear();
}

void Note::clear()
{
  m_id.clear();
  m_number = 0;
  m_supplier.clear();
  m_date = QDate::currentDate();
  m_bCash = false;
  m_vNoteItem.clear();
  m_observation.clear();
  m_disccount = 0.0;
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
      m_bCash != another.m_bCash ||
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

QString Note::SQL_tableName() const
{
  return NOTE_SQL_TABLE_NAME;
}


bool Note::SQL_insert(QString& error)
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_insert_proc(query);
  return SQL_finish(db, query, bSuccess, error);
}

bool Note::SQL_update(QString& error)
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_update_proc(query);
  return SQL_finish(db, query, bSuccess, error);
}

bool Note::SQL_select(QString& error)
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_select_proc(query, error);
  return SQL_finish(db, query, bSuccess, error);
}

bool Note::SQL_remove(QString& error)
{
  error.clear();

  if (!SQL_isOpen(error))
    return false;

  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  db.transaction();
  QSqlQuery query(db);

  bool bSuccess = SQL_remove_proc(query);
  return SQL_finish(db, query, bSuccess, error);
}

bool Note::SQL_insert_proc(QSqlQuery& query)
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
    query.bindValue(":_v04", m_bCash);
    query.bindValue(":_v05", m_observation);
    query.bindValue(":_v06", m_disccount);
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
  return bSuccess;
}

bool Note::SQL_update_proc(QSqlQuery& query)
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
  query.bindValue(":_v04", m_bCash);
  query.bindValue(":_v05", m_observation);
  query.bindValue(":_v06", m_disccount);
  bool bSuccess = query.exec();

  query.prepare("DELETE FROM " NOTE_ITEMS_SQL_TABLE_NAME " WHERE " NOTE_ITEMS_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", m_id.get());
  bSuccess = query.exec();

  if (bSuccess)
  {
    for (int i = 0; i != note.m_vNoteItem.size(); ++i)
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
      else
        break;
    }
  }
  return bSuccess
}

bool Note::SQL_select_proc(QSqlQuery& query, QString& error)
{
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
      m_bCash = query.value(3).toBool();
      m_observation = query.value(4).toString();
      m_disccount = query.value(5).toDouble();
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
        o.push_back(o);
      }
    }
  }

  if (bSuccess)
  {
    QString error2;
    if (m_supplier.m_id.isValid())
      m_supplier.SQL_select_proc(query, error2);
    for (int i = 0; i != m_vNoteItem.size(); ++i)
    {
      if (m_vNoteItem.at(i).m_product.m_id.isValid())
        m_vNoteItem.at(i).m_product.SQL_select_proc(query, error2);
    }
  }

  return bSuccess;
}

bool Note::SQL_remove_proc(QSqlQuery& query)
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
    {
      QString error2;
      o.m_product.SQL_select_proc(query, error2);
    }
  }

  SQL_finish(db, query, bSuccess, error);
  return o;
}
