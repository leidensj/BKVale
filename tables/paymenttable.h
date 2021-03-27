#ifndef PAYMENTTABLE_H
#define PAYMENTTABLE_H

#include "jtable.h"
#include "items/paymentpart.h"

class PaymentTable : public JTable
{
  Q_OBJECT

public:
  enum class Column
  {
    Date,
    Value,
  };

  explicit PaymentTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<PaymentPart>& v) const;
  void set(const QVector<PaymentPart>& v);
  void setPurchaseDate(const QDate& dt);
  QDate getPurchaseDate() const;
  void setPurchaseTotal(double total);
  double getPurchaseTotal() const;
  bool isValid() const;
  double total() const;

public slots:
  void addRow();
  void fill();

private:
  QDate m_dtPurchase;
  double m_purchaseTotal;
};

#endif // PAYMENTTABLE_H
