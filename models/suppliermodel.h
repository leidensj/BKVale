#ifndef SUPPLIERMODEL_H
#define SUPPLIERMODEL_H

#include "jmodel.h"

class SupplierModel : public JModel
{
public:
  SupplierModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // SUPPLIERMODEL_H
