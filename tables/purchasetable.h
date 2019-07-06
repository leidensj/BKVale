#ifndef PURCHASETABLE_H
#define PURCHASETABLE_H

#include "widgets/jtable.h"
#include "items/purchase.h"

#define PURCHASE_TABLE_NUMBER_OF_COLUMNS 5

class PurchaseTable : public JItemTable
{
  Q_OBJECT

public:

  enum class Column : int
  {
    Ammount,
    Unity,
    Description,
    Price,
    SubTotal
  };

  explicit PurchaseTable(QWidget* parent = nullptr);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItemAndLoadPrices(Id supplierId, bool bCode);
  double computeTotal() const;

private:
  mutable PurchaseItem m_ref;
  double computePrice(int row) const;
  double computeSubTotal(int row) const;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // PURCHASETABLE_H
