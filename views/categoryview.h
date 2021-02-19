#ifndef COUPONVIEW_H
#define COUPONVIEW_H

#include "items/coupon.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class DatabasePicker;
class JDatePicker;
class QDateEdit;
class JSpinBox;
class JDoubleSpinBox;
class QRadioButton;

class CouponView : public JItemView
{
  Q_OBJECT

public:
  explicit CouponView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edCode;
  QDateEdit* m_dtCreation;
  QCheckBox* m_cbExpiration;
  JDatePicker* m_dtExpiration;
  QRadioButton* m_rdoPercentage;
  QRadioButton* m_rdoValue;
  QRadioButton* m_rdoProduct;
  JSpinBox* m_spnPercentage;
  JDoubleSpinBox* m_spnValue;
};

#endif // COUPONVIEW_H
