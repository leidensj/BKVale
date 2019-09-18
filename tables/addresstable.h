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
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItem();

private:
  mutable Address m_ref;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // ADDRESSTABLE_H
