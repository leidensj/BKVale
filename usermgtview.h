#ifndef USERMGTVIEW_H
#define USERMGTVIEW_H

#include <QFrame>

class QCheckBox;
class JLineEdit;
class QPushButton;
class QLabel;

class UserMgtView : public QFrame
{
  Q_OBJECT

public:
  explicit UserMgtView(QWidget* parent = 0);

private:
  QPushButton* m_create;
  QPushButton* m_save;
  JLineEdit* m_user;
  JLineEdit* m_password;
  QPushButton* m_viewPassword;
  QCheckBox* m_accessNote;
  QCheckBox* m_accessReminder;
  QCheckBox* m_accessCalculator;
  QCheckBox* m_accessShop;
  QCheckBox* m_accessConsumption;
  QCheckBox* m_accessUser;
  QCheckBox* m_accessItem;
  QCheckBox* m_accessSettings;
};

#endif // USERMGTVIEW_H
