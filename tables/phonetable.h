#ifndef PHONETABLE_H
#define PHONETABLE_H

#include "widgets/jtable.h"
#include "items/phone.h"

class PhoneTable : public JItemTable
{
  Q_OBJECT

  enum class Column
  {
    Phone
  };

public:
  explicit PhoneTable(QWidget* parent = nullptr);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItem();

private:
  mutable Phone m_ref;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // PHONETABLE_H
