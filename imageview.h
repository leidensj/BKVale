#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QFrame>
#include "image.h"
#include <QSqlDatabase>

class QPushButton;
class JLineEdit;
class JImageView;
class JDatabase;

class ImageView : public QFrame
{
  Q_OBJECT

public:
  explicit ImageView(QWidget* parent = 0);
  void setImage(const Image& image);
  Image getImage() const;
  void setDatabase(QSqlDatabase db);

public slots:
  void create();

private slots:
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  void save();

signals:
  saveSignal();

private:
  qlonglong m_currentId;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edImageName;
  JImageView* m_imageView;
  JDatabase* m_database;
};

#endif // IMAGEVIEW_H
