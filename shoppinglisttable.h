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
  QVector<ShoppingListItem> getShopItems() const;
  void setShopItems(const QVector<ShoppingListItem>& vShopItem);
  void setProduct(const Product& product);
  void setShopItem(const ShoppingListItem& shopItem);
  void addShopItem(const ShoppingListItem& shopItem);

  void removeCurrentItem() { removeRow(currentRow()); }
  void removeAllItems() { setRowCount(0); }
  bool hasItems() const { return rowCount() != 0; }

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  QString text(int row, int column) const;
  void setText(int row, int column, const QString& str);
  double evaluate(int row, int column) const;

private slots:
  void update(int row, int column);
  void emitChangedSignal();

signals:
  void changedSignal();
};

#endif // SHOPPINGLISTTABLE_H
