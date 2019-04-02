#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitemsql.h"
#include "defines.h"
#include "category.h"
#include <QRegExp>

struct Package
{
  Package();
  void clear();
  bool operator !=(const Package& other) const;
  QString strFormattedUnity(const QString& productUnity) const;
  QString strUnity(const QString& productUnity) const;

  bool m_bIsPackage;
  QString m_unity;
  double m_ammount;
};

struct ProductCode : public JItem
{
  ProductCode();
  void clear();
  bool isValid() const;
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  QString strTableName() const;

  QString m_code;
};

struct Product : public JItemSQL
{
  Product();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;

  QString SQL_tableName() const;
  bool SQL_insert_proc(QSqlQuery& query) const;
  bool SQL_update_proc(QSqlQuery& query) const;
  bool SQL_select_proc(QSqlQuery& query, QString& error);
  bool SQL_remove_proc(QSqlQuery& query) const;

  bool SQL_select_by_code(const ProductCode& code, QString& error);

  Category m_category;
  Image m_image;
  QString m_name;
  QString m_unity;
  QString m_details;
  bool m_bBuy;
  bool m_bSell;
  QVector<ProductCode> m_vCode;

  QString text() const { return "Produto"; }
  QString icon() const { return ":/icons/res/product.png"; }
  QString name() const { return m_name; }
  QByteArray image() const { return m_image.m_image; }
};

#endif // PRODUCT_H
