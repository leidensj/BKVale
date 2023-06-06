#ifndef INVENTORYTABLE_H
#define INVENTORYTABLE_H

#include "jtable.h"
#include "items/inventoryproduct.h"

class InventoryTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Product,
    Ammount,
    Adjustment,
    Final
  };

  explicit InventoryTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<InventoryProduct>& v) const;
  void set(const QVector<InventoryProduct>& v, bool bClear = true);

public slots:
  void addRow();
};

#endif // INVENTORYTABLE_H
