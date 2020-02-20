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
      REMINDER_SQL_TABLE_NAME;
}

void ReminderModel::select(QHeaderView* header)
{
  JModel::select(getStrQuery());
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Título"));
  setHeaderData(2, Qt::Horizontal, tr("Mensagem"));
  setHeaderData(3, Qt::Horizontal, tr("Assunto"));
  setHeaderData(4, Qt::Horizontal, tr("Data"));
  setHeaderData(5, Qt::Horizontal, tr("Hora"));
  setHeaderData(6, Qt::Horizontal, tr("Favorito"));
  setHeaderData(7, Qt::Horizontal, tr("Data Informada"));
  setHeaderData(8, Qt::Horizontal, tr("Hora Informada"));
  setHeaderData(9, Qt::Horizontal, tr("Maiúsuclas"));
  if (header != nullptr && header->count() == 10)
  {
    header->hideSection(0);
    header->hideSection(2);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(6, QHeaderView::ResizeMode::ResizeToContents);
    header->hideSection(7);
    header->hideSection(8);
    header->hideSection(9);
  }
}

QVariant ReminderModel::data(const QModelIndex &idx, int role) const
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
      bool bDate = record(idx.row()).value(7).toBool();
      value = bDate ? value.toDate().toString("yyyy/MM/dd") : "";
    }
    else if (idx.column() == 5)
    {
      bool bTime = record(idx.row()).value(8).toBool();
      value = bTime ? value.toTime().toString("HH:mm:ss") : "";
    }
    else if (idx.column() == 1)
    {
      auto cap = (Reminder::Capitalization)record(idx.row()).value(9).toInt();
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
    if (idx.column() == 1)
      value = data(idx.sibling(idx.row(), 2), Qt::DisplayRole);
  }
  return value;
}
