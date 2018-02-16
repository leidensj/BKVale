#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

class JDatabase;
class CategoryView;

class CategoryWidget : public QFrame
{
  Q_OBJECT

public:
  explicit CategoryWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);

private slots:
  void categorySelected(int id);
  void removeCategory(int id);
  void saveCategory();
  void searchImage();

private:
  CategoryView* m_view;
  JDatabase* m_database;
};

#endif // CATEGORYWIDGET_H
