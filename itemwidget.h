#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "item.h"

class JDatabase;
class ItemView;

class ItemWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ItemWidget(QWidget *parent = 0);
  ~ItemWidget();
  void setDatabase(QSqlDatabase db);

private slots:
  void itemSelected(int id);
  void removeItem(int id);
  void saveItem();

private:
  JDatabase* m_database;
  ItemView* m_view;
};

#endif // ITEMWIDGET_H
