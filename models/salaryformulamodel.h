#ifndef SALARYFORMULAMODEL_H
#define SALARYFORMULAMODEL_H

#include "jmodel.h"

class SalaryFormulaModel : public JModel
{
public:
  SalaryFormulaModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
};

#endif // SALARYFORMULAMODEL_H
