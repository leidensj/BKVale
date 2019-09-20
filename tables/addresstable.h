#ifndef ADDRESSTABLE_H
#define ADDRESSTABLE_H

#include "widgets/jtable.h"
#include "items/address.h"

class AddressTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Address
  };

public:
  explicit AddressTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void getAddresses(QVector<Address>& v) const;
  void setAddresses(const QVector<Address>& v);

public slots:
  void addRow();
};

#endif // ADDRESSTABLE_H
