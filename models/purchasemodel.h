#ifndef PURCHASEMODEL_H
#define PURCHASEMODEL_H

#include "jmodel.h"

class PurchaseModel : public JModel
{
public:
  PurchaseModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif // PURCHASEMODEL_H
