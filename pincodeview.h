#ifndef PINCODEVIEW_H
#define PINCODEVIEW_H

#include <QDialog>
#include "person.h"

class QPushButton;
class QLineEdit;

class PinCodeView : public QDialog
{
  Q_OBJECT

public:
  explicit PinCodeView(QWidget* parent = nullptr);
  Person getCurrentPerson() const;

protected:
  bool eventFilter(QObject *target, QEvent *event);

private:
  void append(QChar c);

private slots:
  void search();
  void erase();
  void pressed0() { append('0'); }
  void pressed1() { append('1'); }
  void pressed2() { append('2'); }
  void pressed3() { append('3'); }
  void pressed4() { append('4'); }
  void pressed5() { append('5'); }
  void pressed6() { append('6'); }
  void pressed7() { append('7'); }
  void pressed8() { append('8'); }
  void pressed9() { append('9'); }

private:
  Person m_currentPerson;
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

#endif // PINCODEVIEW_H
