#ifndef CASHSUMMARYTABLE_H
#define CASHSUMMARYTABLE_H

#include "jtable.h"
#include "items/cashsummary.h"

class CashSummaryTable : public JTable
{
  Q_OBJECT

  enum class Column
  {
    Name,
    Status
  };

public:
  explicit CashSummaryTable(JAddRemoveButtons* btns = nullptr, QWidget* parent = nullptr);
  void get(QVector<CashSummary>& v) const;
  void set(const QVector<CashSummary>& v, bool bClear = true);

public slots:
  void addRow();
  void addRowAndActivate();
};

#endif
