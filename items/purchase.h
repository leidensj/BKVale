#ifndef PURCHASE_H
#define PURCHASE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDate>
#include "supplier.h"
#include "product.h"
#include "defines.h"
#include "jitemsql.h"
#include "employee.h"
#include "purchaseelement.h"
#include "paymentelement.h"
#include "store.h"

struct Purchase : public JItemSQL
{
  enum class PaymentMethod
  {
    Credit,
    Cash,
    Bonus
  };

  Purchase(Id id = Id());
  void clear(bool bClearId = true);
  bool isValid() const;
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;

  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  void setEmployee(const JItemSQL& e) const;

  QString strDate() const;
  QString strDayOfWeek() const;
  QString strNumber() const;
  QString strDisccount() const;
  double total() const;
  double subTotal() const;
  QString strTotal() const;
  QString strSubTotal() const;
  bool isPaymentValid() const;
  double paymentTotal() const;

  void adjustPayment();

  mutable qlonglong m_number;
  QDate m_date;
  Supplier m_supplier;
  PaymentMethod m_paymentMethod;
  QVector<PaymentElement> m_vPayment;
  QString m_observation;
  QVector<PurchaseElement> m_vElement;
  double m_disccount;
  Store m_store;
  mutable Employee m_employee;
};

#endif // PURCHASE_H
