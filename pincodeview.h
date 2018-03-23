#ifndef PINCODEVIEW_H
#define PINCODEVIEW_H

#include <QDialog>
#include <QSqlDatabase>
#include "person.h"

class QPushButton;
class QLineEdit;

class PinCodeView : public QDialog
{
  Q_OBJECT

public:
  explicit PinCodeView(QWidget* parent = nullptr);
  void setDatabase(QSqlDatabase db);
  Person getCurrentPerson() const;

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  Person m_currentPerson;
  QSqlDatabase m_db;
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
};

#endif // PINCODEVIEW_H
