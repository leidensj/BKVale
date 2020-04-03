#include "remindermodel.h"
#include "items/reminder.h"
#include <QSqlRecord>
#include <QDate>
#include <QTime>

ReminderModel::ReminderModel(QObject *parent)
  : JModel(parent)
{

}

QString ReminderModel::getStrQuery()
{
  return
      "SELECT "
      SQL_COLID ","
      REMINDER_SQL_COL01 ","
      REMINDER_SQL_COL02 ","
      REMINDER_SQL_COL12 ","
      REMINDER_SQL_COL08 ","
      REMINDER_SQL_COL10 ","
      REMINDER_SQL_COL03 ","
      REMINDER_SQL_COL09 ","
      REMINDER_SQL_COL11 ","
      REMINDER_SQL_COL04
      " FROM "
      REMINDER_SQL_TABLE_NAME
      " ORDER BY "
      REMINDER_SQL_COL03 " DESC";
}

void ReminderModel::select(QHeaderView* header)
{
  JModel::select(getStrQuery());
  setHeaderData((int)Column::Id, Qt::Horizontal, tr("ID"));
  setHeaderData((int)Column::Title, Qt::Horizontal, tr("Título"));
  setHeaderData((int)Column::Message, Qt::Horizontal, tr("Mensagem"));
  setHeaderData((int)Column::Subject, Qt::Horizontal, tr("Assunto"));
  setHeaderData((int)Column::Date, Qt::Horizontal, tr("Data"));
  setHeaderData((int)Column::Time, Qt::Horizontal, tr("Hora"));
  setHeaderData((int)Column::Favorite, Qt::Horizontal, tr("Favorito"));
  setHeaderData((int)Column::HasDate, Qt::Horizontal, tr("Data Informada"));
  setHeaderData((int)Column::HasTime, Qt::Horizontal, tr("Hora Informada"));
  setHeaderData((int)Column::Capital, Qt::Horizontal, tr("Maiúsuclas"));
  if (header != nullptr && header->count() == 10)
  {
    header->hideSection((int)Column::Id);
    header->hideSection((int)Column::Message);
    header->setSectionResizeMode((int)Column::Title, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode((int)Column::Subject, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeMode::ResizeToContents);
    header->hideSection((int)Column::Time);
    header->hideSection((int)Column::Favorite);
    header->hideSection((int)Column::HasDate);
    header->hideSection((int)Column::HasTime);
    header->hideSection((int)Column::Capital);
  }
}

QVariant ReminderModel::data(const QModelIndex &idx, int role) const
{
  if (!idx.isValid())
    return QModelIndex();

  QVariant value = QSqlQueryModel::data(idx, role);
  if (role == Qt::DecorationRole)
  {
    if (idx.column() == (int)Column::Title)
    {
      bool bFavorite = data(idx.sibling(idx.row(), (int)Column::Favorite), Qt::EditRole).toBool();
      if (bFavorite)
        value = QVariant::fromValue(QIcon(":/icons/res/favorite.png"));
      else
        value = "";
    }
  }
  else if (role == Qt::DisplayRole)
  {
    if (idx.column() == (int)Column::Date)
    {
      value = tr("%1 %2").arg(record(idx.row()).value((int)Column::HasDate).toBool()
                              ? record(idx.row()).value((int)Column::Date).toDate().toString("yyyy/MM/dd")
                              : "",
                              record(idx.row()).value((int)Column::HasTime).toBool()
                              ? record(idx.row()).value((int)Column::Time).toTime().toString("HH:mm:ss")
                              : "");
    }
    else if (idx.column() == (int)Column::Title)
    {
      auto cap = (Reminder::Capitalization)record(idx.row()).value((int)Column::Capital).toInt();
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
    if (idx.column() == (int)Column::Title)
      value = data(idx.sibling(idx.row(), 2), Qt::DisplayRole);
  }
  return value;
}
