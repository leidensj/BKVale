#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "jitem.h"

class JDatabase;
class PersonView;

class PersonWidget : public QFrame
{
  Q_OBJECT

public:
  explicit PersonWidget(QWidget *parent = 0);
  ~PersonWidget();
  void setDatabase(QSqlDatabase db);

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

private:
  JDatabase* m_database;
  PersonView* m_view;
};

#endif // PERSONWIDGET_H
