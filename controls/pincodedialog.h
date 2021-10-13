#ifndef PINCODE_H
#define PINCODE_H

#include <QDialog>
#include "items/employee.h"

class QPushButton;
class QLineEdit;

class PinCodeDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PinCodeDialog(QWidget* parent = nullptr);
  Employee getEmployee() const;

protected:
  bool eventFilter(QObject *target, QEvent *event);

private slots:
  void search();
  void buttonPressed();
  void erase();
  void append(QChar c);

private:
  bool m_bError;
  Employee m_employee;
  QLineEdit* m_edPinCode;
  QPushButton* m_btn [10];
  QPushButton* m_btnEnter;
  QPushButton* m_btnClr;
};

#endif // PINCODE_H
