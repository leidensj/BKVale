#ifndef CONSUMPTIONWIDGET_H
#define CONSUMPTIONWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include <QDoubleSpinBox>
#include "item.h"
#include "consumption.h"

class QDateEdit;
class QLabel;
class QPushButton;
class QLineEdit;

class ConsumptionDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT

public:
  ConsumptionDoubleSpinBox(QWidget *parent = 0);

protected:
  void keyPressEvent(QKeyEvent* event);
};

class ConsumptionWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ConsumptionWidget(QWidget *parent = 0);
  ~ConsumptionWidget();
  void setDatabase(QSqlDatabase db);
  Consumption consumption() const;

private:
  QDateEdit* m_date;
  QLabel* m_weekDay;
  QPushButton* m_search;
  QLineEdit* m_item;
  QLabel* m_priceIcon;
  ConsumptionDoubleSpinBox* m_price;
  QLabel* m_ammountIcon;
  ConsumptionDoubleSpinBox* m_ammount;
  QPushButton* m_save;
  QSqlDatabase m_db;

private slots:
  void search();
  void dateChanged(const QDate& date);
  void enableControls();
  void itemSelected(const Item& item);
  void save();

signals:
  void consumptionSaved(const Consumption& consumption);
};

#endif // CONSUMPTIONWIDGET_H
