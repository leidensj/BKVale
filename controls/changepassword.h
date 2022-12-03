#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

class JLineEdit;

class ChangePassword : public QDialog
{
  Q_OBJECT

public:
  explicit ChangePassword(QWidget* parent = 0);

private slots:
  void changePassword();

private:
  void updateCapsLock();
  JLineEdit* m_currentPassword;
  JLineEdit* m_newPassword1;
  JLineEdit* m_newPassword2;
  QPushButton* m_btnChange;
};

#endif // CHANGEPASSWORD_H
