#ifndef USERMGTVIEW_H
#define USERMGTVIEW_H

#include <QFrame>
#include "user.h"

class QCheckBox;
class JLineEdit;
class QPushButton;
class QLabel;
class JDatabase;

class UserMgtView : public QFrame
{
  Q_OBJECT

public:
  explicit UserMgtView(qlonglong currentLoggedId, QWidget* parent = 0);
  User getUser() const;
  void setUser(const User& user);
  QString getPassword() const;
  bool hasLoggedUserChanged() const;

private:
  qlonglong m_currentLoggedId;
  int m_currentId;
  bool m_bHasLoggedUserChanged;
  QPushButton* m_create;
  QPushButton* m_save;
  JLineEdit* m_user;
  QLabel* m_lblPasswordMsg;
  JLineEdit* m_password;
  QPushButton* m_viewPassword;
  QCheckBox* m_accessNote;
  QCheckBox* m_accessReminder;
  QCheckBox* m_accessCalculator;
  QCheckBox* m_accessShoppingList;
  QCheckBox* m_accessShop;
  QCheckBox* m_accessUser;
  QCheckBox* m_accessProduct;
  QCheckBox* m_accessPerson;
  QCheckBox* m_accessEmployee;
  QCheckBox* m_accessSupplier;
  QCheckBox* m_accessCategory;
  QCheckBox* m_accessImage;
  QCheckBox* m_accessReservation;
  QCheckBox* m_accessSettings;
  QCheckBox* m_accessProductBarcode;
  JDatabase* m_database;

public slots:
  void create();

private slots:
  void save();
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void viewPassword(bool b);

signals:
  saveSignal();
};

#endif // USERMGTVIEW_H
