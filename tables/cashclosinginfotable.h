#ifndef CASHCLOSINGINFOTABLE_H
#define CASHCLOSINGINFOTABLE_H

#include "jtable.h"
#include "items/cashclosinginfo.h"

class CashClosingInfoTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Info = 0,
    Type,
    Value
  };

  explicit CashClosingInfoTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<CashClosingInfo>& v) const;
  void set(const QVector<CashClosingInfo>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif
