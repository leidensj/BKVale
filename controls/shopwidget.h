#ifndef SHOPVIEW_H
#define SHOPVIEW_H

#include <QFrame>
#include <QDialog>
#include "items/shoppinglist.h"

class DatabaseViewer;
class JDatePicker;
class QLabel;
class QCheckBox;

class ShopWidget : public QWidget
{
  Q_OBJECT

public:
  ShopWidget(QWidget* parent = nullptr);
  ShoppingList getShoppingList();

private slots:
  void updateControls();
  void emitChangedSignal();
  
private:
  DatabaseViewer* m_viewer;
  JDatePicker* m_dt;
  QLabel* m_lblWeekDay;

signals:
  void changedSignal();
};

class ShopPrintDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ShopPrintDialog(QWidget* parent = nullptr);
  bool getCount() const;

private:
  QCheckBox* m_cbCount;
};

#endif // SHOPVIEW_H
