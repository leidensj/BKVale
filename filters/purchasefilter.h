#ifndef PURCHASEFILTERDLG_H
#define PURCHASEFILTERDLG_H

#include "jfilter.h"

class DatabasePicker;
class QCheckBox;
class JDateInterval;

class PurchaseFilter : public JFilter
{
  Q_OBJECT

public:
  explicit PurchaseFilter(QWidget* parent = nullptr);
  virtual QString getFilter() const;

public slots:
  void clear();

protected:
  virtual QString getDateFilter() const;
  virtual QString getSupplierFilter() const;
  virtual QString getProductFilter() const;
  virtual QString getStoreFilter() const;
  virtual QString getPaymentFilter() const;

  JDateInterval* m_dtInt;
  DatabasePicker* m_supplierPicker;
  DatabasePicker* m_productPicker;
  DatabasePicker* m_storePicker;
  QCheckBox* m_cbPaymentCredit;
  QCheckBox* m_cbPaymentCash;
  QCheckBox* m_cbPaymentBonus;
};

#endif // PURCHASEFILTER_H
