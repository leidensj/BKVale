#ifndef COUPONMODEL_H
#define COUPONMODEL_H

#include "jmodel.h"

class CouponModel : public JModel
{
  enum class Column
  {
    Id,
    Type,
    CreationDate,
    RedemptionDate,
    Date,
    Coupon,
    Code,
    Expiration,
    Expired,
    Redemption,
    Redeemed
  };
public:
  CouponModel(QObject *parent);
  QString getStrQuery();
  virtual void select(QHeaderView* header);
  QVariant data(const QModelIndex &idx, int role) const;
};

#endif // CATEGORYMODEL_H
