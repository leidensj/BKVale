#ifndef JIMAGEVIEW_H
#define JIMAGEVIEW_H

#include <QFrame>
#include <QByteArray>
#include "image.h"

class QPushButton;
class QLabel;

class JImageView : public QFrame
{
  Q_OBJECT

public:
  explicit JImageView(QWidget* parent = 0);
  QByteArray getImage();
  bool hasImage() const;

private slots:
  void setImage(const QString& fileName);
  void setImage(const QByteArray& bArray);
  void openImage();
  void clearImage();

private:
  bool m_bHasImage;
  QPushButton* m_btnOpen;
  QPushButton* m_btnClear;
  QLabel* m_lblImage;
  QString getImagePath();
};

#endif // JIMAGEVIEW_H
