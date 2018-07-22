#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <databaseutils.h>

class JLineEdit;
class QLabel;
class QPushButton;

class LoginDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LoginDialog(UserLoginSQL& userLogin,
                       QWidget* parent = 0);

private slots:
  void login();

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  void updateCapsLock();
  UserLoginSQL& m_userLogin;
  JLineEdit* m_user;
  JLineEdit* m_password;
  JLineEdit* m_hostName;
  QLabel* m_capsLock;
  QLabel* m_status;
};

#endif // LOGINDIALOG_H
