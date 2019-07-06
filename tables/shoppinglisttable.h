#ifndef SHOPPINGLISTTABLE_H
#define SHOPPINGLISTTABLE_H

#include <QTableWidget>
#include "items/shoppinglist.h"
#include "widgets/jtable.h"

class ShoppingListTable : public JItemTable
{
  Q_OBJECT

public:
  explicit ShoppingListTable(QWidget* parent = 0);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItem();
  void showSupplierColumn(bool b);

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);

private:
  mutable ShoppingListItem m_ref;
};

#endif // SHOPPINGLISTTABLE_H
