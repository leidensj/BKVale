#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QFrame>
#include "image.h"
#include "jitemview.h"

class QPushButton;
class JLineEdit;
class JImageView;

class ImageView : public JItemView
{
  Q_OBJECT

public:
  explicit ImageView(QWidget* parent = 0);
  void setImage(const Image& image);
  const JItem& getItem() const;

public slots:
  void create();

private:
  JLineEdit* m_edImageName;
  JImageView* m_imageView;
  void setItem(const JItem& o);
};

#endif // IMAGEVIEW_H
