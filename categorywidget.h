#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "jitem.h"

class JDatabase;
class CategoryView;

class CategoryWidget : public QFrame
{
  Q_OBJECT

public:
  explicit CategoryWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

private:
  CategoryView* m_view;
  JDatabase* m_database;
};

#endif // CATEGORYWIDGET_H
