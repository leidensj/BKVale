#ifndef CASHVIEW_H
#define CASHVIEW_H

#include "items/cash.h"
#include "jitemview.h"

class JLineEdit;
class DatabasePicker;
class CashInfoTable;
class CashSummaryTable;
class JAddRemoveButtons;
class QCheckBox;

class CashView : public JItemView
{
  Q_OBJECT

public:
  explicit CashView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edName;
  DatabasePicker* m_sectorPicker;
  DatabasePicker* m_coinPicker;
  CashInfoTable* m_infoTable;
  CashSummaryTable* m_summaryTable;
  JAddRemoveButtons* m_btnAddRemove;
  QCheckBox* m_cbDebit;
  QCheckBox* m_cbCredit;
  QCheckBox* m_cbComission;
};

#endif // CASHVIEW_H
