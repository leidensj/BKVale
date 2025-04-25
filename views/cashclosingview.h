#ifndef CASHCLOSINGVIEW_H
#define CASHCLOSINGVIEW_H

#include "items/cashclosing.h"
#include "jitemview.h"

class JLineEdit;
class DatabasePicker;
class CashClosingCoinTable;
class CashClosingSectorTable;
class CashClosingInfoTable;

class CashClosingView : public JItemView
{
  Q_OBJECT

public:
  explicit CashClosingView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private slots:
  void cashChanged();

private:
  DatabasePicker* m_cashPicker;
  CashClosingCoinTable* m_coinTable;
  CashClosingSectorTable* m_sectorTable;
  CashClosingInfoTable* m_infoTable;
};

#endif // CASHCLOSINGVIEW_H
