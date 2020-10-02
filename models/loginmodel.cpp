#include "loginmodel.h"

LoginModel::LoginModel(QObject *parent)
  : JModel(parent)
{

}

QString LoginModel::getStrQuery()
{
  return
      "SELECT "
      LOGIN_SQL_TABLE_NAME "." SQL_COLID ","
      LOGIN_SQL_TABLE_NAME "." LOGIN_SQL_COL_PID ","
      USER_SQL_TABLE_NAME "." USER_SQL_COL_USE ","
      LOGIN_SQL_TABLE_NAME "." LOGIN_SQL_COL_MAC ","
      LOGIN_SQL_TABLE_NAME "." LOGIN_SQL_COL_LOG
      " FROM "
      LOGIN_SQL_TABLE_NAME " LEFT JOIN "
      USER_SQL_TABLE_NAME " ON "
      LOGIN_SQL_TABLE_NAME "." LOGIN_SQL_COL_UID " = "
      USER_SQL_TABLE_NAME "." SQL_COLID;
}

void LoginModel::select(QHeaderView* header)
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

