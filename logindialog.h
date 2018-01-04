#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class JLineEdit;
class QLabel;
class QPushButton;

class LoginDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LoginDialog(QWidget* parent = 0);

private slots:
  void login();

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  void updateCapsLock();
  JLineEdit* m_user;
  JLineEdit* m_password;
  QLabel* m_capsLock;
  QLabel* m_status;
};

#endif // LOGINDIALOG_H
