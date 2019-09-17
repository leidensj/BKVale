#ifndef PURCHASETABLE_H
#define PURCHASETABLE_H

#include "widgets/jtable.h"
#include "items/purchase.h"

class PurchaseTable : public JTable
{
  Q_OBJECT

public:

  enum class Column : int
  {
    Ammount,
    Unity,
    Product,
    Price,
    SubTotal
  };

  explicit PurchaseTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getPurchases(QVector<PurchaseElement>& v) const;
  void setPurchases(const QVector<PurchaseElement>& v);

public slots:
  void addRow();
  double computeTotal() const;

private:
  double computePrice(int row) const;
  double computeSubTotal(int row) const;

protected slots:
  void update(int row, int column);
};

#endif // PURCHASETABLE_H
