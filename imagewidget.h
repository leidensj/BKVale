#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QFrame>
#include <QSqlDatabase>

class ImageView;
class JDatabase;

class ImageWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ImageWidget(QWidget* parent = 0);
  void setDatabase(QSqlDatabase db);

private slots:
  void imageSelected(int id);
  void removeImage(int id);
  void saveImage();

private:
  ImageView* m_view;
  JDatabase* m_database;
};

#endif // IMAGEWIDGET_H
