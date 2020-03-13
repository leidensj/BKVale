#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H

#include "jtableitem.h"
#include "items/product.h"

class PackageItem : public JTableItem
{
public:
  PackageItem();
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);
  void setProductUnity(const QString& productUnity);

  static QVariant st_toVariant(const Package& pck);
  static Package st_fromVariant(const QVariant& v);
private:
  QString m_productUnity;
};

#endif // PACKAGEITEM_H
