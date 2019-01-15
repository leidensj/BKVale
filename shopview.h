#ifndef SHOPVIEW_H
#define SHOPVIEW_H

#include <QFrame>
#include "shoppinglist.h"

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
  bool printCount() const;

private slots:
  void updateControls();
  void setToday();
  void emitChangedSignal();
  
private:
  JDatabase* m_database;
  QDateEdit* m_dtDate;
  QPushButton* m_btnToday;
  QLabel* m_lblWeekDay;
  QCheckBox* m_cbPrintCount;

signals:
  void changedSignal();
};

#endif // SHOPVIEW_H
