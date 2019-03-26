#ifndef DISCOUNT_H
#define DISCOUNT_H

#include "jitem.h"
#include "product.h"
#include <QVector>
#include <QMetaType>
#include <QDate>

struct DiscountItem : JItem
{
  enum class Column
  {
    Ammount,
    Unity,
    Name
  };

  DiscountItem();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  QString strTableName() const;
  bool isValid() const;
  QString strAmmount() const;

  Product m_product;
  double m_ammount;
};

Q_DECLARE_METATYPE(DiscountItem)

struct Discount : public SQL_JItem
{
  enum class Type
  {
    None = 0,
    Value,
    Percentage,
    Product
  };

  Discount();
  void clear();
  bool operator !=(const JItem& other) const;
  bool operator ==(const JItem& other) const;
  bool isValid() const;
  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query);
  bool SQL_update_proc(QSqlQuery& query);
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query);
  bool SQL_redeem(const QString& code, bool& redeemed, QString& error);

  static QString strType(Type type);
  QString strExpDate() const;
  static QString getRandomCode(const int length = 10);

  mutable QString m_code;
  Type m_type;
  double m_value;
  double m_percentage;
  QVector<DiscountItem> m_items;
  bool m_bRedeemed;
  bool m_bExpires;
  QDate m_dtExp;
  QString m_description;
};

#endif // DISCOUNT_H
