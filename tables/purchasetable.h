#ifndef PURCHASETABLE_H
#define PURCHASETABLE_H

#include "widgets/jtable.h"
#include "items/purchase.h"

class PurchaseTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Ammount,
    Unity,
    Product,
    Price,
    SubTotal
  };

public:
  explicit PurchaseTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getPurchaseElements(QVector<PurchaseElement>& v) const;
  void setPurchaseElements(const QVector<PurchaseElement>& v);

public slots:
  void addRow();
  void addRowAndActivate();
  void addRowByCode();
  double computeTotal() const;
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
