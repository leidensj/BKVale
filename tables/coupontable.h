#ifndef COUPONTABLE_H
#define COUPONTABLE_H

#include "jtable.h"
#include "items/couponproduct.h"

class CouponTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Ammount,
    Product
  };

  explicit CouponTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<CouponProduct>& v) const;
  void set(const QVector<CouponProduct>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif // PURCHASETABLE_H
