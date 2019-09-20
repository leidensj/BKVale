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
  explicit ShoppingListTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getListItems(QVector<ShoppingListItem>& v) const;
  void setListItems(const QVector<ShoppingListItem>& v);

public slots:
  void addRow();
  void showSupplierColumn(bool b);
};

#endif // SHOPPINGLISTTABLE_H
