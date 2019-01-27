#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>
#include <QVector>
#include "jitem.h"
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

struct Product : public JItem
{
  Product();
  void clear();
  bool operator != (const JItem& other) const;
  bool operator == (const JItem& other) const;
  bool isValid() const;
  QString strTableName() const;

  Category m_category;
  Image m_image;
  QString m_name;
  QString m_unity;
  QString m_details;
  bool m_bBuy;
  bool m_bSell;
  QVector<ProductCode> m_vCode;
};

#endif // PRODUCT_H
