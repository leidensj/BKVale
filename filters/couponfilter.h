#ifndef COUPONFILTER_H
#define COUPONFILTER_H

#include "jfilter.h"

class QCheckBox;
class JDateInterval;

class CouponFilter : public JFilter
{
  Q_OBJECT

public:
  explicit CouponFilter(QWidget* parent = nullptr);
  virtual QString getFilter() const;

public slots:
  virtual void clear();

private:
  QCheckBox* m_cbExpired;
  QCheckBox* m_cbRedeemed;
  QCheckBox* m_cbPercentage;
  QCheckBox* m_cbValue;
  QCheckBox* m_cbProduct;
  JDateInterval* m_dtCreation;
  JDateInterval* m_dtRedemption;
};

#endif // COUPONFILTER_H
