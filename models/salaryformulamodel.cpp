#include "salaryformulamodel.h"

SalaryFormulaModel::SalaryFormulaModel(QObject *parent)
  : JModel(parent)
{

}

QString SalaryFormulaModel::getStrQuery()
{
  QString strQuery("SELECT "
                   SQL_COLID ","
                   SALARY_FORMULA_SQL_COL_NAM
                   " FROM "
                   SALARY_FORMULA_SQL_TABLE_NAME);
  return strQuery;
}

void SalaryFormulaModel::select(QHeaderView* header)
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
