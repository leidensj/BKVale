#ifndef DISCOUNTMODEL_H
#define DISCOUNTMODEL_H

#include "jmodel.h"

class DiscountModel : public JModel
{
public:
  DiscountModel(QObject *parent);
  QString getStrQuery();
  void select(QHeaderView* header);
  QVariant data(const QModelIndex &index, int role) const;
};

#endif // DISCOUNTMODEL_H
