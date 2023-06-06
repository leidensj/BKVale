#ifndef INVENTORYMODEL_H
#define INVENTORYMODEL_H

#include "jmodel.h"

class InventoryModel : public JModel
{
public:
  InventoryModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
};

#endif // INVENTORYMODEL_H
