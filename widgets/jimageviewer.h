#ifndef JIMAGEVIEWER_H
#define JIMAGEVIEWER_H

class QPushButton;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

#include <QWidget>

class JImageViewer : public QWidget
{
  Q_OBJECT

public:
  JImageViewer(bool bReadOnly = false, QWidget *parent = nullptr);
  bool loadFile(const QString& fileName);
  void setImage(const QImage& image);
  QByteArray getCompressedImageAsByteArray() const;

private slots:
  void open();
  void copy();
  void paste();
  void zoomIn();
  void zoomOut();
  void normalSize();
  void fitToWindow();

private:
  void createActions();
  void createMenus();
  void updateActions();
  void scaleImage(double factor);
  void adjustScrollBar(QScrollBar *scrollBar, double factor);

  QImage m_image;
  QLabel *m_imageLabel;
  QScrollArea *m_scrollArea;
  double m_scaleFactor;

  QPushButton* m_btnOpen;
  QPushButton* m_btnCopy;
  QPushButton* m_btnPaste;
  QPushButton* m_btnZoomIn;
  QPushButton* m_btnZoomOut;
  QPushButton* m_btnNormalSize;
  QPushButton* m_btnFit;
};

#endif
