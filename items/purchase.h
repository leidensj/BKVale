#ifndef PURCHASE_H
#define PURCHASE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDate>
#include <QIcon>
#include "supplier.h"
#include "product.h"
#include "defines.h"
#include "jitemsql.h"
#include "employee.h"
#include "purchaseproduct.h"
#include "paymentpart.h"
#include "store.h"

struct Purchase : public JItemSQL
{
  enum class PaymentMethod
  {
    Credit,
    Cash,
    Bonus
  };

  static QIcon st_paymentIcon(PaymentMethod e);
  static QString st_paymentText(PaymentMethod e);

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
  bool SQL_select_all_supplier_id_items();
  QByteArray printVersion(const QVariant& arg) const;

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
  QVector<PaymentPart> m_payments;
  QString m_observation;
  QVector<PurchaseProduct> m_products;
  double m_disccount;
  Store m_store;
  mutable Employee m_employee;
};

#endif // PURCHASE_H
