#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QFrame>

class ImageView;

class ImageWidget : public QFrame
{
  Q_OBJECT

public:
  explicit ImageWidget(QWidget* parent = 0);

private:
  ImageView* m_view;
};

#endif // IMAGEWIDGET_H
