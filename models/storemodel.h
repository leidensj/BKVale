#ifndef STOREMODEL_H
#define STOREMODEL_H

#include "jmodel.h"

class StoreModel : public JModel
{
public:
  StoreModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // STOREMODEL_H
