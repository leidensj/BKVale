#ifndef COUPONFILTER_H
#define COUPONFILTER_H

#include "jfilter.h"

class QCheckBox;

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
};

#endif // COUPONFILTER_H
