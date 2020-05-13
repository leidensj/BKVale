#include "employeemodel.h"

EmployeeModel::EmployeeModel(QObject *parent)
  : JModel(parent)
{

}

QString EmployeeModel::getStrQuery()
{
  QString strQuery("SELECT "
                   EMPLOYEE_SQL_TABLE_NAME "." SQL_COLID ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL_NAM ","
                   FORM_SQL_TABLE_NAME "." FORM_SQL_COL_ALI
                   " FROM " EMPLOYEE_SQL_TABLE_NAME
                   " LEFT OUTER JOIN "
                   FORM_SQL_TABLE_NAME
                   " ON " EMPLOYEE_SQL_TABLE_NAME "." EMPLOYEE_SQL_COL_FID " = "
                   FORM_SQL_TABLE_NAME "." SQL_COLID);
  return strQuery;
}

void EmployeeModel::select(QHeaderView* header)
{
  JModel::select(getStrQuery());
  setHeaderData(0, Qt::Horizontal, tr("ID"));
  setHeaderData(1, Qt::Horizontal, tr("Nome"));
  setHeaderData(2, Qt::Horizontal, tr("Apelido"));
  if (header != nullptr)
  {
    if (header->count() == 3)
    {
      header->hideSection(0);
      header->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
      header->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    }
  }
}

