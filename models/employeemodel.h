#ifndef EMPLOYEEMODEL_H
#define EMPLOYEEMODEL_H

#include "jmodel.h"

class EmployeeModel : public JModel
{
public:
  EmployeeModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // EMPLOYEEMODEL_H
