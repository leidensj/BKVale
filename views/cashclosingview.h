#ifndef CASHCLOSINGVIEW_H
#define CASHCLOSINGVIEW_H

#include "items/cashclosing.h"
#include "jitemview.h"

class JExpLineEdit;
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
  void save();

protected slots:
  void setItem(const JItemSQL& o);

private slots:
  void cashChanged();
  void update();
  void showCalculator();
  void showPostit();

private:
  DatabasePicker* m_cashPicker;
  JLineEdit* m_dt;
  CashClosingCoinTable* m_coinTable;
  CashClosingSectorTable* m_sectorTable;
  CashClosingInfoTable* m_infoTable;
  JExpLineEdit* m_edSector1;
  JExpLineEdit* m_edSector2;
  JExpLineEdit* m_edCoin1;
  JExpLineEdit* m_edCoin2;
  JExpLineEdit* m_edDiff1;
  JExpLineEdit* m_edDiff2;
  QPushButton* m_btnCalc;
  QPushButton* m_btnPostit;
};

#endif // CASHCLOSINGVIEW_H
