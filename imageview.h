#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QFrame>

class QPushButton;
class JLineEdit;
class JImageView;

class ImageView : public QFrame
{
  Q_OBJECT

public:
  explicit ImageView(QWidget* parent = 0);

private:
  QPushButton* m_btnCreate;
  QPushButton* m_btnSave;
  JLineEdit* m_edImageName;
  JImageView* m_imageView;
};

#endif // IMAGEVIEW_H
