#ifndef CASHCLOSINGMODEL_H
#define CASHCLOSINGMODEL_H

#include "jmodel.h"

class CashClosingModel : public JModel
{
  enum class Column
  {
    Id,
    Date,
    Cash,
    Sales,
    Gross,
    Net,
    Tax,
    Discrepancy,
    Difference
  };
public:
  CashClosingModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif
