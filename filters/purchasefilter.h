#ifndef PURCHASEFILTERDLG_H
#define PURCHASEFILTERDLG_H

#include "widgets/jfilter.h"

class JDatabasePicker;
class JDateEdit;
class QGroupBox;
class QCheckBox;

class PurchaseFilter : public JFilter
{
  Q_OBJECT

public:
  explicit PurchaseFilter(QWidget* parent = nullptr);
  QString getFilter() const;

public slots:
  void clear();

private slots:
  void updateControls();

private:
  QGroupBox* m_cbDate;
  JDateEdit* m_dtBegin;
  JDateEdit* m_dtEnd;
  JDatabasePicker* m_supplierPicker;
  JDatabasePicker* m_productPicker;
  JDatabasePicker* m_storePicker;
  QCheckBox* m_cbPaymentCredit;
  QCheckBox* m_cbPaymentCash;
  QCheckBox* m_cbPaymentBonus;
};

#endif // PURCHASEFILTER_H
