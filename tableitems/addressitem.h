#ifndef ADDRESSITEM_H
#define ADDRESSITEM_H

#include "jtableitem.h"
#include "items/address.h"

class AddressItem : public JTableItem
{
public:
  AddressItem();
  void evaluate();
  void erase();
  void activate();
  void setValue(const QVariant& v);

  static QVariant toVariant(const Address& o);
  static Address toAddress(const QVariant& v);
};

#endif // ADDRESSITEM_H
