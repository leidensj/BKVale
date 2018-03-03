#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

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
  void personSelected(int id);
  void removePerson(int id);
  void savePerson();
  void searchImage();

private:
  JDatabase* m_database;
  PersonView* m_view;
};

#endif // PERSONWIDGET_H
