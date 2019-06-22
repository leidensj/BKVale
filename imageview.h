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

public slots:
  void create();
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private:
  JLineEdit* m_edImageName;
  JImageView* m_imageView;
};

#endif // IMAGEVIEW_H
