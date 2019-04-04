#ifndef USERMGTVIEW_H
#define USERMGTVIEW_H

#include <QFrame>
#include "user.h"
#include "jitemview.h"

class QCheckBox;
class JLineEdit;
class QLabel;

class UserMgtView : public JItemView
{
  Q_OBJECT

public:
  explicit UserMgtView(Id currentLoggedId, QWidget* parent = 0);
  const JItemSQL& getItem() const;

  QString getPassword() const;
  bool hasLoggedUserChanged() const;

public slots:
  void create();

protected slots:
  void itemsRemoved(const QVector<Id>& ids);
  void save();

private:
  mutable User m_ref;
  Id m_currentLoggedId;
  bool m_bHasLoggedUserChanged;
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
  QCheckBox* m_accessForm;
  QCheckBox* m_accessEmployee;
  QCheckBox* m_accessSupplier;
  QCheckBox* m_accessCategory;
  QCheckBox* m_accessImage;
  QCheckBox* m_accessReservation;
  QCheckBox* m_accessSettings;

  void setItem(const JItemSQL& o);

private slots:
  void viewPassword(bool b);
};

#endif // USERMGTVIEW_H
