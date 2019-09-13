#ifndef PHONETABLE_H
#define PHONETABLE_H

#include "widgets/jtable.h"
#include "items/phone.h"

class PhoneTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    CountryCode,
    Code,
    Number,
    Name
  };

public:
  explicit PhoneTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void addRow();
  void getPhones(QVector<Phone>& v) const;
  void setPhones(const QVector<Phone>& v);
};

#endif // PHONETABLE_H
