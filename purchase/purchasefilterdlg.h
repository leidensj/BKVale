#ifndef PURCHASEFILTERDLG_H
#define PURCHASEFILTERDLG_H

#include "filterdlg.h"

class JDatabasePicker;
class QDateEdit;
class QCheckBox;

class PurchaseFilterDlg : public FilterDlg
{
  Q_OBJECT

public:
  explicit PurchaseFilterDlg(QWidget* parent = nullptr);
  virtual QString getFilter() const;
  virtual void clearFilter();

private slots:
  void updateControls();

private:
  QCheckBox* m_cbDate;
  QDateEdit* m_dtBegin;
  QDateEdit* m_dtEnd;
  JDatabasePicker* m_supplierPicker;
  JDatabasePicker* m_productPicker;
};

#endif // PURCHASEFILTERDLG_H
