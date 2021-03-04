#ifndef COUPONVIEW_H
#define COUPONVIEW_H

#include "items/coupon.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class JDatePicker;
class JDateEdit;
class QRadioButton;
class JExpLineEdit;
class QLabel;
class JSpinBox;

class CouponView : public JItemView
{
  Q_OBJECT

public:
  explicit CouponView(QWidget* parent = 0);

public slots:
  void getItem(JItemSQL& o) const;
  bool save(Id& id);

protected slots:
  void setItem(const JItemSQL& o);

private slots:
  void updateControls();

private:
  QLabel* m_lblRedeemed;
  JLineEdit* m_edCode;
  QCheckBox* m_cbExpiration;
  JDatePicker* m_dtExpiration;
  QRadioButton* m_rdoPercentage;
  QRadioButton* m_rdoValue;
  JExpLineEdit* m_edPercentage;
  JExpLineEdit* m_edValue;
};

#endif // CATEGORYVIEW_H
