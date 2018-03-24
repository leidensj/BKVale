#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QFrame>
#include <QSqlDatabase>
#include "jitem.h"

class ImageView;
class JDatabase;

class ImageWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ImageWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

private:
  ImageView* m_view;
  JDatabase* m_database;
};

#endif // IMAGEWIDGET_H
