#ifndef USERMGTWIDGET_H
#define USERMGTWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

class UserMgtView;
class UserMgtDatabase;
class JDatabase;

class UserMgtWidget : public QFrame
{
  Q_OBJECT

public:
  explicit UserMgtWidget(int currentUserID, QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  bool hasAnyUserChanged() const;

private:
  const int m_currentUserID;
  UserMgtView* m_view;
  JDatabase* m_database;
  bool m_bHasAnyUserChanged;

private slots:
  void userSelected(int id);
  void removeUser(int id);
  void saveUser();
};

#endif // USERMGTWIDGET_H
