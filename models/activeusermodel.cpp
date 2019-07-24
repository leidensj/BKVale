#include "activeusermodel.h"

ActiveUserModel::ActiveUserModel(QObject *parent)
  : JModel(parent)
{

}

QString ActiveUserModel::getStrQuery()
{
  return
      "SELECT "
      ACTIVE_USERS_SQL_TABLE_NAME "." SQL_COLID ","
      ACTIVE_USERS_SQL_TABLE_NAME "." ACTIVE_USERS_SQL_COL01 ","
      USER_SQL_TABLE_NAME "." USER_SQL_COL01 ","
      ACTIVE_USERS_SQL_TABLE_NAME "." ACTIVE_USERS_SQL_COL03 ","
      ACTIVE_USERS_SQL_TABLE_NAME "." ACTIVE_USERS_SQL_COL04
      " FROM "
      ACTIVE_USERS_SQL_TABLE_NAME " LEFT JOIN "
      USER_SQL_TABLE_NAME " ON "
      ACTIVE_USERS_SQL_TABLE_NAME "." ACTIVE_USERS_SQL_COL02 " = "
      USER_SQL_TABLE_NAME "." SQL_COLID;
}

void ActiveUserModel::select(QHeaderView* header)
{
  JModel::select(getStrQuery());
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("PID"));
  setHeaderData(2, Qt::Horizontal, tr("Usuário"));
  setHeaderData(3, Qt::Horizontal, tr("Máquina"));
  setHeaderData(4, Qt::Horizontal, tr("Login"));
  if (header != nullptr && header->count() == 5)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);
    header->setSectionResizeMode(4, QHeaderView::ResizeMode::ResizeToContents);
  }
}

