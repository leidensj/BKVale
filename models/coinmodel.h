#ifndef COINMODEL_H
#define COINMODEL_H

#include "jmodel.h"

class CoinModel : public JModel
{
  enum class Column
  {
    Id,
    Name,
    Tax,
    Image
  };

public:
  CoinModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif // COINMODEL_H
