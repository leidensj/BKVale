#ifndef CASHMODEL_H
#define CASHMODEL_H

#include "jmodel.h"

class CashModel : public JModel
{
  enum class Column
  {
    Id,
    Name
  };
public:
  CashModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif
