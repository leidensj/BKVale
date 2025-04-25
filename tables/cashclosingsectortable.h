#ifndef CASHCLOSINGSECTORTABLE_H
#define CASHCLOSINGSECTORTABLE_H

#include "jtable.h"
#include "items/cashclosingsector.h"

class CashClosingSectorTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Sector = 0,
    Value,
    NValue
  };

  explicit CashClosingSectorTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<CashClosingSector>& v) const;
  void set(const QVector<CashClosingSector>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif
