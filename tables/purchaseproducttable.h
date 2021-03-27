#ifndef PURCHASEPRODUCTTABLE_H
#define PURCHASEPRODUCTTABLE_H

#include "jtable.h"
#include "items/purchase.h"

class PurchaseProductTable : public JTable
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

  explicit PurchaseProductTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<PurchaseProduct>& v) const;
  void set(const QVector<PurchaseProduct>& v, bool bClear = true);

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

#endif // PURCHASEPRODUCTTABLE_H
