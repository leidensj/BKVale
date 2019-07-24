#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include "jmodel.h"

class CategoryModel : public JModel
{
public:
  CategoryModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
};

#endif // CATEGORYMODEL_H
