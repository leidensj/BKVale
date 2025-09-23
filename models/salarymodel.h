#ifndef SALARYMODEL_H
#define SALARYMODEL_H

#include "jmodel.h"

class SalaryModel : public JModel
{
public:
  SalaryModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
};

#endif // SALARYMODEL_H
