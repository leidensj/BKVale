#ifndef PINCODE_H
#define PINCODE_H

#include <QDialog>
#include "items/employee.h"

class QPushButton;
class QLineEdit;

class PinCodeView : public QDialog
{
  Q_OBJECT

public:
  explicit PinCodeView(QWidget* parent = nullptr);
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
  QPushButton* m_btn0;
  QPushButton* m_btn1;
  QPushButton* m_btn2;
  QPushButton* m_btn3;
  QPushButton* m_btn4;
  QPushButton* m_btn5;
  QPushButton* m_btn6;
  QPushButton* m_btn7;
  QPushButton* m_btn8;
  QPushButton* m_btn9;
  QPushButton* m_btnEnter;
  QPushButton* m_btnClr;
};

#endif // PINCODE_H
