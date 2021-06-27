#ifndef COUPONVIEW_H
#define COUPONVIEW_H

#include "items/coupon.h"
#include "jitemview.h"
#include <QDialog>

class QPushButton;
class JLineEdit;
class JDatePicker;
class JDateEdit;
class QRadioButton;
class JExpLineEdit;
class QLabel;
class JSpinBox;
class DatabasePicker;
class JAddRemoveButtons;
class CouponTable;
class CouponFilter;

class CouponConfirmation : public QDialog
{
  Q_OBJECT

public:
  explicit CouponConfirmation(const QVector<Coupon>& coupons, QWidget* parent = 0);
};

class CouponView : public JItemView
{
  Q_OBJECT

public:
  explicit CouponView(QWidget* parent = nullptr);

public slots:
  void getItem(JItemSQL& o) const;
  void save();

protected slots:
  void setItem(const JItemSQL& o);

private slots:
  static bool st_saveMultiple(QVector<Coupon>& v, QWidget* parent);
  void updateControls();

private:
  QLabel* m_lblRedeemed;
  JLineEdit* m_edCode;
  QCheckBox* m_cbExpiration;
  JDatePicker* m_dtExpiration;
  QRadioButton* m_rdoPercentage;
  QRadioButton* m_rdoValue;
  QRadioButton* m_rdoProduct;
  JExpLineEdit* m_edPercentage;
  JExpLineEdit* m_edValue;
  DatabasePicker* m_storePicker;
  JAddRemoveButtons* m_btnAddRemove;
  CouponTable* m_table;
  QLabel* m_lblCount;
  CouponFilter* m_filter;
};

#endif // CATEGORYVIEW_H
