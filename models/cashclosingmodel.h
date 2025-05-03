#ifndef CASHCLOSINGMODEL_H
#define CASHCLOSINGMODEL_H

#include "jmodel.h"

class CashClosingModel : public JModel
{
  enum class Column
  {
    Id,
    Cash,
    Date,
    Sales
  };
public:
  CashClosingModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif
