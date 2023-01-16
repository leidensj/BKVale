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
  void enableControls();
  void print();
  void view();

private:
  DatabaseViewer* m_viewer;
  JDatePicker* m_dt;
  QPushButton* m_btnPrint;
  QPushButton* m_btnView;
};

#endif // SHOPVIEW_H
