#ifndef USERMGTWIDGET_H
#define USERMGTWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "jitem.h"

class UserMgtView;
class UserMgtDatabase;
class JDatabase;

class UserMgtWidget : public QFrame
{
  Q_OBJECT

public:
  explicit UserMgtWidget(qlonglong currentUserID, QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);
  bool hasAnyUserChanged() const;

private:
  const qlonglong m_currentUserID;
  UserMgtView* m_view;
  JDatabase* m_database;
  bool m_bHasAnyUserChanged;

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();
};

#endif // USERMGTWIDGET_H
