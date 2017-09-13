#ifndef CONSUMPTIONVIEW_H
#define CONSUMPTIONVIEW_H

#include <QFrame>
#include <QSqlDatabase>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "item.h"
#include "consumption.h"

class QDateEdit;
class QLabel;
class QLineEdit;

class EnterDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT

public:
  EnterDoubleSpinBox(QWidget *parent = 0);

protected:
  void keyPressEvent(QKeyEvent* event);
};

class EnterPushButton : public QPushButton
{
  Q_OBJECT

public:
  EnterPushButton(QWidget *parent = 0);

protected:
  void keyPressEvent(QKeyEvent* event);
};

class ConsumptionView : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionView(QWidget *parent = 0);
  ~ConsumptionView();
  void setDatabase(QSqlDatabase db);
  Consumption consumption() const;

private:
  int m_currentItemID;
  QDateEdit* m_date;
  QLabel* m_weekDay;
  EnterPushButton* m_search;
  QLineEdit* m_item;
  EnterDoubleSpinBox* m_price;
  EnterDoubleSpinBox* m_ammount;
  EnterPushButton* m_save;
  QSqlDatabase m_db;

private slots:
  void search();
  void dateChanged(const QDate& date);
  void enableControls();
  void itemSelected(const Item& item);
  void save();
  void clear();

signals:
  void insertSignal(const Consumption& consumption);
};

#endif // CONSUMPTIONVIEW_H
