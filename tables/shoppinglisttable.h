#ifndef SHOPPINGLISTTABLE_H
#define SHOPPINGLISTTABLE_H

#include <QTableWidget>
#include "items/shoppinglist.h"
#include "widgets/jtable.h"

class ShoppingListTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Package,
    Product,
    Ammount,
    Price,
    Supplier
  };

public:
  explicit ShoppingListTable(JAddRemoveButtons* btns = nullptr, bool bSelector = false, QWidget* parent = nullptr);
  void getListElements(QVector<ShoppingListItem>& v) const;
  void setListElements(const QVector<ShoppingListItem>& v);

public slots:
  void addRow();
  void addRowAndActivate();
  void showSupplierColumn(bool b);

private slots:
  void update(int row, int column);
};

#endif // SHOPPINGLISTTABLE_H
