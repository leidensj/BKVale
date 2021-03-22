#ifndef SHOPVIEW_H
#define SHOPVIEW_H

#include <QFrame>
#include "items/shoppinglist.h"

class DatabaseViewer;
class JDatePicker;
class QPushButton;

class ShopWidget : public QWidget
{
  Q_OBJECT

public:
  ShopWidget(QWidget* parent = nullptr);
  ShoppingList getShoppingList();

private slots:
  void updateControls();
  void print();

private:
  DatabaseViewer* m_viewer;
  JDatePicker* m_dt;
  QPushButton* m_btnPrint;
};

#endif // SHOPVIEW_H
