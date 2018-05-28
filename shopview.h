#ifndef SHOPVIEW_H
#define SHOPVIEW_H

#include <QFrame>
#include <QSqlDatabase>
#include "shoppinglist.h"
#include "settings.h"
#include <QIODevice>

class JDatabase;
class QDateEdit;
class QPushButton;
class QLabel;

class ShopView : public QFrame
{
  Q_OBJECT

public:
  ShopView(QWidget* parent = nullptr);
  void setDatabase(QSqlDatabase db);
  ShoppingList getShoppingList() const;
  void print(QIODevice* printer, InterfaceType type);

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

#endif // SHOPVIEW_H
