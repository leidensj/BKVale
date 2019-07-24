#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include "jmodel.h"

class ProductModel : public JModel
{
public:
  ProductModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // PRODUCTMODEL_H
