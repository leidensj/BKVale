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
  bool isDirty() const;
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
  bool m_bHasImage;
  bool m_bDirty;
  QPushButton* m_btnOpen;
  QPushButton* m_btnClear;
  QLabel* m_lblImage;
  QString getImagePath();
};

#endif // JIMAGEVIEW_H
