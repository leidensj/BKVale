#ifndef SHOPPINGLISTMODEL_H
#define SHOPPINGLISTMODEL_H

#include "jmodel.h"

class ShoppingListModel : public JModel
{
public:
  ShoppingListModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
};

#endif // SHOPPINGLISTMODEL_H
