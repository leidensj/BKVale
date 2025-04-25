#ifndef CASHCLOSINGCOINTABLE_H
#define CASHCLOSINGCOINTABLE_H

#include "jtable.h"
#include "items/cashclosingcoin.h"

class CashClosingCoinTable : public JTable
{
  Q_OBJECT

public:

  enum class Column
  {
    Coin = 0,
    Tax,
    Value
  };

  explicit CashClosingCoinTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<CashClosingCoin>& v) const;
  void set(const QVector<CashClosingCoin>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif
