#include "payment.h"
#include "jmodel.h"
#include <QDate>

class PaymentModel : public JModel
{
public:
  PaymentModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    QString strQuery("SELECT "
                     SQL_COLID ","
                     PAYMENT_SQL_TABLE_NAME "." PAYMENT_SQL_COL02 ","
                     PAYMENT_SQL_TABLE_NAME "." PAYMENT_SQL_COL01 ","
                     FORM_SQL_TABLE_NAME "." FORM_SQL_COL02 ","
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL01 ","
                     " FROM "
                     PAYMENT_SQL_TABLE_NAME
                     " LEFT OUTER JOIN "
                     NOTE_SQL_TABLE_NAME
                     " ON " PAYMENT_SQL_TABLE_NAME "." PAYMENT_SQL_COL03 " = "
                     NOTE_SQL_TABLE_NAME "." SQL_COLID
                     " LEFT OUTER JOIN "
                     SUPPLIER_SQL_TABLE_NAME " ON "
                     NOTE_SQL_TABLE_NAME "." NOTE_SQL_COL03 " = "
                     SUPPLIER_SQL_TABLE_NAME "." SQL_COLID
                     " LEFT OUTER JOIN "
                     FORM_SQL_TABLE_NAME " ON "
                     SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL01 " = "
                     FORM_SQL_TABLE_NAME "." SQL_COLID);
    return strQuery;
  }

  virtual void select(QHeaderView* header)
  {
    JModel::select("");
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Data"));
    setHeaderData(2, Qt::Horizontal, tr("Valor"));
    setHeaderData(3, Qt::Horizontal, tr("Fornecedor"));
    setHeaderData(4, Qt::Horizontal, tr("Número"));
    if (header != nullptr && header->count() == 5)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
    }
  }
};


PaymentPart::PaymentPart()
{
  clear();
}

void PaymentPart::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_value = 0.0;
  m_date = QDate::currentDate();
}

bool PaymentPart::operator != (const JItem& other) const
{
  const PaymentPart& another = dynamic_cast<const PaymentPart&>(other);
  return m_value != another.m_value ||
         m_date != another.m_date;
}

bool PaymentPart::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool PaymentPart::isValid() const
{
  return true;
}

Payment::Payment()
{
  clear();
}

void Payment::clear(bool bClearId)
{
  if (bClearId)
  {
    m_id.clear();
    m_noteId.clear();
  }
  m_cash = 0.0;
  m_bonus = 0.0;
  m_vCredit.clear();
}

bool Payment::operator != (const JItem& other) const
{
  const Payment& another = dynamic_cast<const Payment&>(other);
  return m_cash != another.m_cash ||
         m_bonus != another.m_bonus ||
         m_vCredit != another.m_vCredit;
}

bool Payment::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Payment::isValid() const
{
  return true;
}

QString Payment::SQL_tableName() const
{
  return PAYMENT_SQL_TABLE_NAME;
}

bool Payment::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " PAYMENT_SQL_TABLE_NAME " ("
                PAYMENT_SQL_COL01 ","
                PAYMENT_SQL_COL02 ","
                PAYMENT_SQL_COL03 ")"
                " VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03))");

  query.bindValue(":_v01", m_noteId.get());
  query.bindValue(":_v02", m_cash);
  query.bindValue(":_v03", m_bonus);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    m_id.set(query.lastInsertId().toLongLong());
    for (int i = 0; i != m_vCredit.size() && bSuccess; ++i)
    {
      query.prepare("INSERT INTO " PAYMENT_PARTS_SQL_TABLE_NAME " ("
                    PAYMENT_PARTS_SQL_COL01 ","
                    PAYMENT_PARTS_SQL_COL02 ","
                    PAYMENT_PARTS_SQL_COL03 ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");

      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vCredit.at(i).m_date);
      query.bindValue(":_v03",  m_vCredit.at(i).m_value);
      bSuccess = query.exec();
      if (bSuccess)
        m_vCredit[i].m_id.set(query.lastInsertId().toLongLong());
    }
  }
  return bSuccess;
}

bool Payment::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " PAYMENT_SQL_TABLE_NAME " SET "
                PAYMENT_SQL_COL01 " = (:_v01),"
                PAYMENT_SQL_COL02 " = (:_v02),"
                PAYMENT_SQL_COL03 " = (:_v03)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_noteId.get());
  query.bindValue(":_v02", m_cash);
  query.bindValue(":_v03", m_bonus);

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    query.prepare("DELETE FROM " PAYMENT_PARTS_SQL_TABLE_NAME
                  " WHERE " PAYMENT_PARTS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    for (int i = 0; i != m_vCredit.size() && bSuccess; ++i)
    {
      query.prepare("INSERT INTO " PAYMENT_PARTS_SQL_TABLE_NAME " ("
                    PAYMENT_PARTS_SQL_COL01 ","
                    PAYMENT_PARTS_SQL_COL02 ","
                    PAYMENT_PARTS_SQL_COL03 ")"
                    " VALUES ("
                    "(:_v01),"
                    "(:_v02),"
                    "(:_v03))");
      query.bindValue(":_v01", m_id.get());
      query.bindValue(":_v02", m_vCredit.at(i).m_date);
      query.bindValue(":_v03",  m_vCredit.at(i).m_value);
      bSuccess = query.exec();
      if (bSuccess)
        m_vCredit[i].m_id.set(query.lastInsertId().toLongLong());
    }
  }

  return bSuccess;
}

bool Payment::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();
  clear(false);

  query.prepare("SELECT "
                PAYMENT_SQL_COL01 ","
                PAYMENT_SQL_COL02 ","
                PAYMENT_SQL_COL03
                " FROM " PAYMENT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_noteId.set(query.value(0).toLongLong());
      m_cash = query.value(1).toDouble();
      m_bonus = query.value(2).toDouble();
    }
    else
    {
      error = "Pagamento não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
  {
    query.prepare("SELECT "
                  SQL_COLID ","
                  PAYMENT_PARTS_SQL_COL02 ","
                  PAYMENT_PARTS_SQL_COL03
                  " FROM " PAYMENT_PARTS_SQL_TABLE_NAME
                  " WHERE " PAYMENT_PARTS_SQL_COL01 " = (:_v01)");
    query.bindValue(":_v01", m_id.get());
    bSuccess = query.exec();
    if (bSuccess)
    {
      while (query.next())
      {
        PaymentPart o;
        o.m_id.set(query.value(0).toLongLong());
        o.m_date = query.value(1).toDate();
        o.m_value = query.value(2).toDouble();
        m_vCredit.push_back(o);
      }
    }
  }

  return bSuccess;
}

bool Payment::SQL_select_proc_by_noteid(QSqlQuery& query, QString& error)
{
  query.prepare("SELECT "
                SQL_COLID
                " FROM " PAYMENT_SQL_TABLE_NAME
                " WHERE " PAYMENT_SQL_COL01 " = (:_v01)");
  query.bindValue(":_v01", m_noteId.get());

  bool bSuccess = query.exec();
  if (bSuccess)
  {
    if (query.next())
    {
      m_id.set(query.value(0).toLongLong());
    }
    else
    {
      error = "Pagamento não encontrado.";
      bSuccess = false;
    }
  }

  if (bSuccess)
    bSuccess = SQL_select_proc(query, error);
  return bSuccess;
}

bool Payment::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " PAYMENT_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

JModel* Payment::SQL_table_model(QObject* parent) const
{
  return new PaymentModel(parent);
}

double Payment::total() const
{
  double t = m_cash + m_bonus;
  for (int i = 0; i != m_vCredit.size(); ++i)
    t += m_vCredit.at(i).m_value;
  return t;
}

bool Payment::isAllCash(double total) const
{
  return st_areEqual(m_cash, total, DataType::Money);
}

bool Payment::isAllBonus(double total) const
{
  return st_areEqual(m_bonus, total, DataType::Money);
}

bool Payment::isAllCredit(double total) const
{
  double sum = 0.0;
  for (int i = 0; i != m_vCredit.size(); ++i)
    sum += m_vCredit.at(i).m_value;
  return st_areEqual(sum, total, DataType::Money);
}
