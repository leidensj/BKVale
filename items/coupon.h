#ifndef COUPON_H
#define COUPON_H

#include <QString>
#include <QObject>
#include <QDate>
#include "jitemsql.h"
#include "store.h"

struct Coupon : public JItemSQL
{
  enum class Type
  {
    Percentage = 0,
    Value,
    Product
  };

  Coupon(Id id = Id());
  void clear(bool bClearId = true);
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;
  bool SQL_redeem(QString& error);

  static QString st_strType(Type type);
  QString strType() const;
  QString strCoupon() const;

  static QString st_newCode();

  Type m_type;
  QString m_code;
  QDateTime m_dtCreation;
  bool m_bRedeemed;
  QDateTime m_dtRedeemed;
  bool m_bExpires;
  QDate m_dtExpiration;
  int m_percentage;
  double m_value;
  Store m_store;
};

#endif // COUPON_H
