#ifndef COUPONREDEEMER_H
#define COUPONREDEEMER_H

#include <QDialog>

class JLineEdit;

class CouponRedeemer : public QDialog
{
  Q_OBJECT
public:
  explicit CouponRedeemer(QWidget* parent = nullptr);

private slots:
  void redeem();

private:

  JLineEdit* m_edCode;
};

#endif // COUPONREDEEMER_H
