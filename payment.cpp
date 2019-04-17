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

Payment::Payment()
{
  clear();
}

void Payment::clear()
{
  m_id.clear();
  m_value = 0.0;
  m_date = QDate::currentDate();
  m_noteId.clear();
}

bool Payment::operator != (const JItem& other) const
{
  const Payment& another = dynamic_cast<const Payment&>(other);
  return m_value != another.m_value ||
         m_date != another.m_date;
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

  query.bindValue(":_v01", m_value);
  query.bindValue(":_v02", m_date);
  query.bindValue(":_v03", m_noteId.get());

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());

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
  query.bindValue(":_v01", m_value);
  query.bindValue(":_v02", m_date);
  query.bindValue(":_v03", m_noteId.get());

  return query.exec();
}

bool Payment::SQL_select_proc(QSqlQuery& query, QString& error)
{
  error.clear();

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
      m_value = query.value(0).toDouble();
      m_date = query.value(1).toDate();
      m_noteId.set(query.value(3).toLongLong());
    }
    else
    {
      error = "Pagamento não encontrado.";
      bSuccess = false;
    }
  }

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
