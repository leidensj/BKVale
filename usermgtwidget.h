#ifndef USERMGTWIDGET_H
#define USERMGTWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

class UserMgtView;
class UserMgtDatabase;

class UserMgtWidget : public QFrame
{
  Q_OBJECT

public:
  explicit UserMgtWidget(int currentUserID, QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);

private:
  const int m_currentUserID;
  UserMgtView* m_view;
  UserMgtDatabase* m_database;

private slots:
  void setUser(int id);
  void removeUser(int id);
  void save();
};

#endif // USERMGTWIDGET_H
