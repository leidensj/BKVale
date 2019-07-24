#ifndef PRODUCTCODEMODEL_H
#define PRODUCTCODEMODEL_H

#include "jmodel.h"

class ProductCodeModel : public JModel
{
public:
  ProductCodeModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
};

#endif // PRODUCTCODEMODEL_H
