#ifndef USERVIEW_H
#define USERVIEW_H

#include <QFrame>
#include "items/user.h"
#include "jitemview.h"

class QCheckBox;
class JLineEdit;
class QLabel;
class QListWidget;

class UserView : public JItemView
{
  Q_OBJECT

public:
  explicit UserView(QWidget* parent = 0);


  QString getPassword() const;
  bool hasLoggedUserChanged() const;

public slots:
  void getItem(JItemSQL& o) const;
  void save();

protected slots:
  void itemsRemoved(const Ids& ids);
  void setItem(const JItemSQL& o);

private:
  bool m_bHasLoggedUserChanged;
  JLineEdit* m_user;
  QLabel* m_lblPasswordMsg;
  JLineEdit* m_password;
  QPushButton* m_viewPassword;
  QListWidget* m_list;

private slots:
  void viewPassword(bool b);
};

#endif // USERVIEW_H
