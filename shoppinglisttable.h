#ifndef SHOPPINGLISTTABLE_H
#define SHOPPINGLISTTABLE_H

#include <QTableWidget>
#include "shoppinglist.h"

class QKeyEvent;

class ShoppingListTable : public QTableWidget
{
  Q_OBJECT

public:
  explicit ShoppingListTable(QWidget* parent = 0);
  QVector<ShoppingListItem> getShoppingItems() const;
  void setShoppingItems(const QVector<ShoppingListItem>& vItem);
  void setProduct(const Product& product);
  void setPackage(const Package& package);
  void setShoppingItem(const ShoppingListItem& shopItem);
  void addShoppingItem(const ShoppingListItem& shopItem);

  void removeCurrentItem() { removeRow(currentRow()); }
  void removeAllItems() { setRowCount(0); }
  bool hasItems() const { return rowCount() != 0; }

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  double evaluate(int row, int column) const;

private slots:
  void update(int row, int column);
  void emitChangedSignal();
  void emitEditSignal(int row, int column);

signals:
  void changedSignal();
  void packageSignal(const Package& package,
                     const QString& productUnity);
  void productSignal(const Product& product);
};

#endif // SHOPPINGLISTTABLE_H
