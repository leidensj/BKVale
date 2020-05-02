#ifndef PURCHASEFILTERDLG_H
#define PURCHASEFILTERDLG_H

#include "jfilter.h"

class DatabasePicker;
class JDateEdit;
class QGroupBox;
class QCheckBox;

class PurchaseFilter : public JFilter
{
  Q_OBJECT

public:
  explicit PurchaseFilter(QWidget* parent = nullptr);
  virtual QString getFilter() const;

public slots:
  void clear();

private slots:
  void updateControls();

protected:
  virtual QString getDateFilter() const;
  virtual QString getSupplierFilter() const;
  virtual QString getProductFilter() const;
  virtual QString getStoreFilter() const;
  virtual QString getPaymentFilter() const;

  QGroupBox* m_cbDate;
  JDateEdit* m_dtBegin;
  JDateEdit* m_dtEnd;
  DatabasePicker* m_supplierPicker;
  DatabasePicker* m_productPicker;
  DatabasePicker* m_storePicker;
  QCheckBox* m_cbPaymentCredit;
  QCheckBox* m_cbPaymentCash;
  QCheckBox* m_cbPaymentBonus;
};

#endif // PURCHASEFILTER_H
