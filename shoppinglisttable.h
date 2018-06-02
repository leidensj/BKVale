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
  void setShoppingItem(const ShoppingListItem& shopItem);
  void addShoppingItem(const ShoppingListItem& shopItem);

  void removeCurrentItem() { removeRow(currentRow()); }
  void removeAllItems() { setRowCount(0); }
  bool hasItems() const { return rowCount() != 0; }

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  QString text(int row, int column) const;
  void setText(int row, int column, const QString& str);
  double evaluate(int row, int column) const;
  void setUnityEnabled(bool bEnable);

private slots:
  void update(int row, int column);
  void emitChangedSignal();

signals:
  void changedSignal();
};

#endif // SHOPPINGLISTTABLE_H
