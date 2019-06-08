#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QFrame>
#include "items/image.h"
#include "widgets/jitemview.h"

class QPushButton;
class JLineEdit;
class JImageView;

class ImageView : public JItemView
{
  Q_OBJECT

public:
  explicit ImageView(QWidget* parent = 0);
  void setImage(const JItemSQL& image);
  const JItemSQL& getItem() const;
  Id getId() const;

public slots:
  void create();

private:
  mutable Image m_ref;
  JLineEdit* m_edImageName;
  JImageView* m_imageView;
  void setItem(const JItemSQL& o);
};

#endif // IMAGEVIEW_H
