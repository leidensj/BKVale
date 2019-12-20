#include "paymenttable.h"
#include <QHeaderView>
#include "tableitems/doubleitem.h"
#include "tableitems/dateitem.h"

PaymentTable::PaymentTable(JAddRemoveButtons* btns, bool bSelector, QWidget* parent)
  : JTable(btns, bSelector, parent)
  , m_purchaseTotal(0.0)
{
  setColumnCount(2);
  QStringList headers;
  headers << "Data" << "Valor";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::Date, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Value, QHeaderView::Stretch);
}

void PaymentTable::setPurchaseDate(const QDate& dt)
{
  m_dtPurchase = dt;
}

QDate PaymentTable::getPurchaseDate() const
{
  return m_dtPurchase;
}

void PaymentTable::setPurchaseTotal(double total)
{
  m_purchaseTotal = total;
}

double PaymentTable::getPurchaseTotal() const
{
  return m_purchaseTotal;
}

double PaymentTable::total() const
{
  return sum((int)Column::Value);
}

void PaymentTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itDate = new DateItem(m_dtPurchase, DateItem::Color::DateBeforeDefault);
  auto itValue = new DoubleItem(Data::Type::Money, DoubleItem::Color::Foreground);

  blockSignals(true);
  setItem(row, (int)Column::Date, itDate);
  setItem(row, (int)Column::Value, itValue);
  blockSignals(false);

  getItem(row, (int)Column::Date)->setValue(m_dtPurchase.addMonths(row));


  setCurrentItem(itDate);
  setFocus();
}

void PaymentTable::getPaymentElements(QVector<PaymentElement>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    int row = verticalHeader()->logicalIndex(row);
    PaymentElement o;
    o.m_date = getItem(row, (int)Column::Date)->getValue().toDate();
    o.m_value = getItem(row, (int)Column::Value)->getValue().toDouble();
    v.push_back(o);
  }
}

void PaymentTable::setPaymentElements(const QVector<PaymentElement>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::Date)->setValue(v.at(i).m_date);
    getItem(i, (int)Column::Value)->setValue(v.at(i).m_value);
  }
}

void PaymentTable::fill()
{
  removeAllItems();
  addRow();
  getItem(rowCount() - 1, (int)Column::Date)->setValue(m_dtPurchase);
<<<<<<< HEAD
  getItem(rowCount() - 1, (int)Column::Value)->setValue(m_purchaseTotal);
}

bool PaymentTable::isValid() const
{
  bool bValid = Data::areEqual(m_purchaseTotal, total(), Data::Type::Money);
  for (int i = 0; i != rowCount() && bValid; ++i)
    bValid = getItem(i, (int)Column::Date)->getValue().toDate() >= m_dtPurchase;
  return bValid;
=======
  getItem(rowCount() - 1, (int)Column::Value)->setValue(total);
>>>>>>> origin/master
}
