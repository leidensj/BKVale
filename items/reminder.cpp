#include "reminder.h"
#include "jmodel.h"
#include <QSqlRecord>

class ReminderModel : public JModel
{
public:
  ReminderModel(QObject *parent)
    : JModel(parent)
  {

  }

  QString getStrQuery()
  {
    return
        "SELECT "
        SQL_COLID ","
        REMINDER_SQL_COL01 ","
        REMINDER_SQL_COL02 ","
        REMINDER_SQL_COL12 ","
        REMINDER_SQL_COL08 ","
        REMINDER_SQL_COL10 ","
        REMINDER_SQL_COL03
        " FROM "
        REMINDER_SQL_TABLE_NAME;
  }

  void select(QHeaderView* header)
  {
    JModel::select(getStrQuery());
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Título"));
    setHeaderData(2, Qt::Horizontal, tr("Mensagem"));
    setHeaderData(3, Qt::Horizontal, tr("Assunto"));
    setHeaderData(4, Qt::Horizontal, tr("Data"));
    setHeaderData(5, Qt::Horizontal, tr("Hora"));
    setHeaderData(6, Qt::Horizontal, tr("Favorito"));
    if (header != nullptr && header->count() == 7)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::Fixed);
      header->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
      header->setSectionResizeMode(6, QHeaderView::ResizeMode::ResizeToContents);
    }
  }

  QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
  {
    if (!idx.isValid())
      return QModelIndex();

    QVariant value = QSqlQueryModel::data(idx, role);
    if (role == Qt::DecorationRole)
    {
      if (idx.column() == 6)
      {
        if (QSqlQueryModel::data(idx, Qt::EditRole).toBool())
          value = QVariant::fromValue(QIcon(":/icons/res/favorite.png"));
        else
          value = "";
      }
    }
    else if (role == Qt::DisplayRole)
    {
      if (idx.column() == 6)
        value = value.toBool() ? tr("Sim") : "";
      else if (idx.column() == 4)
      {
        bool bDate = record(idx.row()).value(9).toBool();
        value = bDate ? value.toDate().toString("yyyy/MM/dd") : "";
      }
      else if (idx.column() == 5)
      {
        bool bTime = record(idx.row()).value(11).toBool();
        value = bTime ? value.toTime().toString("HH:mm:ss") : "";
      }
      else if (idx.column() == 1)
      {
        auto cap = (Reminder::Capitalization)record(idx.row()).value(4).toInt();
        switch (cap)
        {
          case Reminder::Capitalization::AllUppercase:
            value = value.toString().toUpper();
            break;
          case Reminder::Capitalization::AllLowercase:
            value = value.toString().toLower();
            break;
          case Reminder::Capitalization::Normal:
          default:
            break;
        }
      }
    }
    else if (role == Qt::ToolTipRole)
    {
      if (idx.column() == 2)
        value = data(idx, Qt::DisplayRole);
    }
    return value;
  }
};

Reminder::Reminder()
{
  clear();
}

void Reminder::clear(bool bClearId)
{
  if (bClearId)
    m_id.clear();
  m_title.clear();
  m_message.clear();
  m_bFavorite = false;
  m_size = Size::Large;
  m_capitalization = Capitalization::AllUppercase;
  m_bBarcodeHRI = true;
  m_barcode.clear();
  m_date = QDate::currentDate();
  m_bDate = false;
  m_time = QTime::currentTime();
  m_time = m_time.addSecs((-1)*m_time.second());
  m_bTime = false;
  m_subject.clear();
}

bool Reminder::operator != (const JItem& other) const
{
  const Reminder& another = dynamic_cast<const Reminder&>(other);
  return m_title != another.m_title ||
         m_message != another.m_message ||
         m_bFavorite != another.m_bFavorite ||
         m_size != another.m_size ||
         m_capitalization != another.m_capitalization ||
         m_bBarcodeHRI != another.m_bBarcodeHRI ||
         m_barcode != another.m_barcode ||
         m_date != another.m_date ||
         m_bDate != another.m_bDate ||
         m_time != another.m_time ||
         m_bTime != another.m_bTime ||
         m_subject != another.m_subject;
}

bool Reminder::operator == (const JItem& other) const
{
  return !(*this != other);
}

bool Reminder::isValid() const
{
  return !m_title.isEmpty() || !m_message.isEmpty() || !m_barcode.isEmpty();
}

QString Reminder::SQL_tableName() const
{
  return REMINDER_SQL_TABLE_NAME;
}

bool Reminder::SQL_insert_proc(QSqlQuery& query) const
{
  query.prepare("INSERT INTO " REMINDER_SQL_TABLE_NAME " ("
                REMINDER_SQL_COL01 ","
                REMINDER_SQL_COL02 ","
                REMINDER_SQL_COL03 ","
                REMINDER_SQL_COL04 ","
                REMINDER_SQL_COL05 ","
                REMINDER_SQL_COL06 ","
                REMINDER_SQL_COL07 ","
                REMINDER_SQL_COL08 ","
                REMINDER_SQL_COL09 ","
                REMINDER_SQL_COL10 ","
                REMINDER_SQL_COL11 ","
                REMINDER_SQL_COL12
                ") VALUES ("
                "(:_v01),"
                "(:_v02),"
                "(:_v03),"
                "(:_v04),"
                "(:_v05),"
                "(:_v06),"
                "(:_v07),"
                "(:_v08),"
                "(:_v09),"
                "(:_v10),"
                "(:_v11),"
                "(:_v12))");
  query.bindValue(":_v01", m_title);
  query.bindValue(":_v02", m_message);
  query.bindValue(":_v03", m_bFavorite);
  query.bindValue(":_v04", (int)m_capitalization);
  query.bindValue(":_v05", (int)m_size);
  query.bindValue(":_v06", m_bBarcodeHRI);
  query.bindValue(":_v07", m_barcode);
  query.bindValue(":_v08", m_date);
  query.bindValue(":_v09", m_bDate);
  query.bindValue(":_v10", m_time);
  query.bindValue(":_v11", m_bTime);
  query.bindValue(":_v12", m_subject);

  bool bSuccess = query.exec();
  if (bSuccess)
    m_id.set(query.lastInsertId().toLongLong());
  return bSuccess;
}

bool Reminder::SQL_update_proc(QSqlQuery& query) const
{
  query.prepare("UPDATE " REMINDER_SQL_TABLE_NAME " SET "
                REMINDER_SQL_COL01 " = (:_v01),"
                REMINDER_SQL_COL02 " = (:_v02),"
                REMINDER_SQL_COL03 " = (:_v03),"
                REMINDER_SQL_COL04 " = (:_v04),"
                REMINDER_SQL_COL05 " = (:_v05),"
                REMINDER_SQL_COL06 " = (:_v06),"
                REMINDER_SQL_COL07 " = (:_v07),"
                REMINDER_SQL_COL08 " = (:_v08),"
                REMINDER_SQL_COL09 " = (:_v09),"
                REMINDER_SQL_COL10 " = (:_v10),"
                REMINDER_SQL_COL11 " = (:_v11),"
                REMINDER_SQL_COL12 " = (:_v12)"
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  query.bindValue(":_v01", m_title);
  query.bindValue(":_v02", m_message);
  query.bindValue(":_v03", m_bFavorite);
  query.bindValue(":_v04", (int)m_capitalization);
  query.bindValue(":_v05", (int)m_size);
  query.bindValue(":_v06", m_bBarcodeHRI);
  query.bindValue(":_v07", m_barcode);
  query.bindValue(":_v08", m_date);
  query.bindValue(":_v09", m_bDate);
  query.bindValue(":_v10", m_time);
  query.bindValue(":_v11", m_bTime);
  query.bindValue(":_v12", m_subject);
  return query.exec();
}

bool Reminder::SQL_select_proc(QSqlQuery& query, QString& error)
{
  query.prepare("SELECT "
                REMINDER_SQL_COL01 ","
                REMINDER_SQL_COL02 ","
                REMINDER_SQL_COL03 ","
                REMINDER_SQL_COL04 ","
                REMINDER_SQL_COL05 ","
                REMINDER_SQL_COL06 ","
                REMINDER_SQL_COL07 ","
                REMINDER_SQL_COL08 ","
                REMINDER_SQL_COL09 ","
                REMINDER_SQL_COL10 ","
                REMINDER_SQL_COL11 ","
                REMINDER_SQL_COL12
                " FROM " REMINDER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  bool bSuccess = query.exec();

  if (bSuccess)
  {
    if (query.next())
    {
      m_title = query.value(0).toString();
      m_message = query.value(1).toString();
      m_bFavorite = query.value(2).toBool();
      m_capitalization = (Reminder::Capitalization)query.value(3).toInt();
      m_size = (Reminder::Size)query.value(4).toInt();
      m_bBarcodeHRI = query.value(5).toBool();
      m_barcode = query.value(6).toString();
      m_date = query.value(7).toDate();
      m_bDate = query.value(8).toBool();
      m_time = query.value(9).toTime();
      m_bTime = query.value(10).toBool();
      m_subject = query.value(11).toString();
    }
    else
    {
      error = "Lembrete não encontrado.";
      bSuccess = false;
    }
  }

  return bSuccess;
}

bool Reminder::SQL_remove_proc(QSqlQuery& query) const
{
  query.prepare("DELETE FROM " REMINDER_SQL_TABLE_NAME
                " WHERE " SQL_COLID " = (:_v00)");
  query.bindValue(":_v00", m_id.get());
  return query.exec();
}

QStringList Reminder::SQL_select_subjects()
{
  QStringList lst;
  QString error;
  if (SQL_isOpen(error))
  {
    QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
    db.transaction();
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT "
                  REMINDER_SQL_COL12
                  " FROM " REMINDER_SQL_TABLE_NAME);
    bool bSuccess = query.exec();
    while (bSuccess && query.next())
      lst.push_back(query.value(0).toString());
    SQL_finish(db, query, bSuccess, error);
  }
  return lst;
}

JModel* Reminder::SQL_table_model(QObject* parent) const
{
  return new ReminderModel(parent);
}
