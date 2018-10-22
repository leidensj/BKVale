#ifndef SHOPPINGLISTTABLE_H
#define SHOPPINGLISTTABLE_H

#include <QTableWidget>
#include "shoppinglist.h"
#include "jitemtable.h"

class ShoppingListTable : public JTable
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
  void itemDoubleClicked(int row, int column);
  void deletePressed(int row, int column);

private:
  mutable ShoppingListItem m_ref;
};

#endif // SHOPPINGLISTTABLE_H
