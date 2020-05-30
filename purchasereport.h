#ifndef PURCHASEREPORT_H
#define PURCHASEREPORT_H

#include <QWidget>
#include "filters/purchasefilter.h"

class QPushButton;
class QRadioButton;

class PurchaseReport : public PurchaseFilter
{
  Q_OBJECT

public:
  explicit PurchaseReport(QWidget* parent = nullptr);

public slots:
  QString process() const;
  virtual void clear();

protected:
  virtual QString getProductFilter() const;

private:
  QString strFilterHtml() const;
  QString processPurchase() const;
  QString processProduct() const;
  QRadioButton* m_rdoPurchase;
  QRadioButton* m_rdoProduct;
  QRadioButton* m_rdoSupplier;
  QString getQuery() const;
};

#endif // PURCHASEREPORT_H
