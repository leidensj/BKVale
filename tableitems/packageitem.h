#ifndef PACKAGEITEM_H
#define PACKAGEITEM_H

#include "jtableitem.h"

class PackageItem : public JTableItem
{
public:
  PackageItem();
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);
private:
  QString* m_productUnity;
};

#endif // PACKAGEITEM_H
