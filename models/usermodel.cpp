#include "usermodel.h"

UserModel::UserModel(QObject *parent)
  : JModel(parent)
{

}

QString UserModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   USER_SQL_COL_USE
                   " FROM "
                   USER_SQL_TABLE_NAME);
  return strQuery;
}

void UserModel::select(QHeaderView* header)
{
  JModel::select("");
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Nome"));
  if (header != nullptr && header->count() == 2)
  {
    header->hideSection(0);
    header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
  }
}
