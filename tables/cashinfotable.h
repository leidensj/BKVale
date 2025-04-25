#ifndef CASHINFOTABLE_H
#define CASHINFOTABLE_H

#include "jtable.h"
#include "items/cashinfo.h"

class CashInfoTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Name,
    Type
  };

  explicit CashInfoTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<CashInfo>& v) const;
  void set(const QVector<CashInfo>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif
