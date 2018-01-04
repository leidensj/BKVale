#ifndef USERMGTWIDGET_H
#define USERMGTWIDGET_H

#include <QFrame>

class UserMgtView;
class UserMgtDatabase;

class UserMgtWidget : public QFrame
{
public:
  explicit UserMgtWidget(QWidget* parent = 0);

private:
  UserMgtView* m_view;
  UserMgtDatabase* m_database;
};

#endif // USERMGTWIDGET_H
