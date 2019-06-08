#ifndef JIMAGEVIEW_H
#define JIMAGEVIEW_H

#include <QFrame>
#include <QByteArray>
#include "items/image.h"

class QPushButton;
class QLabel;

class JImageView : public QFrame
{
  Q_OBJECT

public:
  explicit JImageView(bool bShowButtons, int size = 64, QWidget* parent = 0);
  QByteArray getImage();
  bool hasImage() const;
  void setImage(const QByteArray& bArray);

public slots:
  void clearImage();

private slots:
  void setImage(const QString& fileName);
  void openImage();
  void updateControls();

signals:
  changedSignal();

private:
  const int m_size;
  bool m_bHasImage;
  QPushButton* m_btnOpen;
  QPushButton* m_btnClear;
  QLabel* m_lblImage;
  QString getImagePath();
};

#endif // JIMAGEVIEW_H
