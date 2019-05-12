#ifndef SHOPVIEW_H
#define SHOPVIEW_H

#include <QFrame>
#include <QDialog>
#include "items/shoppinglist.h"

class JDatabase;
class QDateEdit;
class QPushButton;
class QLabel;
class QCheckBox;

class ShopView : public QFrame
{
  Q_OBJECT

public:
  ShopView(QWidget* parent = nullptr);
  ShoppingList getShoppingList() const;

private slots:
  void updateControls();
  void setToday();
  void emitChangedSignal();
  
private:
  JDatabase* m_database;
  QDateEdit* m_dtDate;
  QPushButton* m_btnToday;
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
