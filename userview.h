#ifndef USERVIEW_H
#define USERVIEW_H

#include <QFrame>
#include "items/user.h"
#include "widgets/jitemview.h"

class QCheckBox;
class JLineEdit;
class QLabel;
class QListWidget;

class UserView : public JItemView
{
  Q_OBJECT

public:
  explicit UserView(Id currentLoggedId, QWidget* parent = 0);
  const JItemSQL& getItem() const;
  Id getId() const;

  QString getPassword() const;
  bool hasLoggedUserChanged() const;

public slots:
  void create();

protected slots:
  void itemsRemoved(const QVector<Id>& ids);
  void save();

private:
  enum class Idx : int
  {
    User =0,
    Category,
    Product,
    Image,
    Form,
    Employee,
    Supplier,
    Store,
    Note,
    Calculator,
    Reminder,
    ShoppingList,
    Shop,
    Reservation,
    Discount,
    TimeCard,
    Settings
  };

  static QString idxToTableName(Idx idx);

  mutable User m_ref;
  Id m_currentLoggedId;
  bool m_bHasLoggedUserChanged;
  JLineEdit* m_user;
  QLabel* m_lblPasswordMsg;
  JLineEdit* m_password;
  QPushButton* m_viewPassword;
  QListWidget* m_list;

  void setItem(const JItemSQL& o);

private slots:
  void viewPassword(bool b);
};

#endif // USERVIEW_H
