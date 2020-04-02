#ifndef PURCHASETABLE_H
#define PURCHASETABLE_H

#include "jtable.h"
#include "items/purchase.h"

class PurchaseTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Ammount,
    Package,
    Product,
    Price,
    SubTotal
  };

  explicit PurchaseTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getPurchaseElements(QVector<PurchaseElement>& v) const;
  void setPurchaseElements(const QVector<PurchaseElement>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
  void addRowByCode();
  void setSupplierId(Id id);

private:
  double computePrice(int row) const;
  double computeSubTotal(int row) const;
  void loadProductInfo(int row);
  Id m_supplierId;

protected slots:
  void update(int row, int column);
};

#endif // PURCHASETABLE_H
