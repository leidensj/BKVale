#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QFrame>
#include "image.h"

class QPushButton;
class JLineEdit;
class JImageView;

class ImageView : public QFrame
{
  Q_OBJECT

public:
  explicit ImageView(QWidget* parent = 0);
  void setImage(const Image& image);
  Image getImage() const;

public slots:
  void create();

private slots:
  void emitSaveSignal();
  void updateControls();
  void setDirty();

signals:
  saveSignal();

private:
  int m_currentId;
  bool m_bDirty;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edImageName;
  JImageView* m_imageView;
};

#endif // IMAGEVIEW_H
