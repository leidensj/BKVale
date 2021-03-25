#ifndef COUPONTABLE_H
#define COUPONTABLE_H

#include "jtable.h"
#include "items/couponelement.h"

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
  void getElements(QVector<CouponElement>& v) const;
  void setElements(const QVector<CouponElement>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif // PURCHASETABLE_H
