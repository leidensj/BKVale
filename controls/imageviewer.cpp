#include "imageviewer.h"
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QPushButton>
#include <QLayout>
#include <QBuffer>

ImageViewer::ImageViewer(bool bReadOnly, QWidget *parent)
   : QWidget(parent)
   , m_imageLabel(nullptr)
   , m_scrollArea(nullptr)
   , m_scaleFactor(1)
   , m_btnOpen(nullptr)
   , m_btnCopy(nullptr)
   , m_btnPaste(nullptr)
   , m_btnZoomIn(nullptr)
   , m_btnZoomOut(nullptr)
   , m_btnNormalSize(nullptr)
   , m_btnFit(nullptr)
{
  m_imageLabel = new QLabel;
  m_imageLabel->setBackgroundRole(QPalette::Base);
  m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_imageLabel->setScaledContents(true);

  m_scrollArea = new QScrollArea;
  m_scrollArea->setBackgroundRole(QPalette::Dark);
  m_scrollArea->setWidget(m_imageLabel);
  m_scrollArea->setVisible(false);

  m_btnOpen = new QPushButton;
  m_btnOpen->setFlat(true);
  m_btnOpen->setIconSize(QSize(24, 24));
  m_btnOpen->setIcon(QIcon(":/icons/res/open.png"));
  m_btnOpen->setShortcut(QKeySequence::Open);
  m_btnOpen->setToolTip(tr("Abrir"));
  m_btnOpen->setEnabled(!bReadOnly);
  m_btnOpen->setVisible(!bReadOnly);

  m_btnCopy = new QPushButton;
  m_btnCopy->setFlat(true);
  m_btnCopy->setIconSize(QSize(24, 24));
  m_btnCopy->setIcon(QIcon(":/icons/res/copy.png"));
  m_btnCopy->setShortcut(QKeySequence::Copy);
  m_btnCopy->setToolTip(tr("Copiar"));
  m_btnCopy->setEnabled(!bReadOnly);
  m_btnCopy->setVisible(!bReadOnly);

  m_btnPaste = new QPushButton;
  m_btnPaste->setFlat(true);
  m_btnPaste->setIconSize(QSize(24, 24));
  m_btnPaste->setIcon(QIcon(":/icons/res/paste.png"));
  m_btnPaste->setShortcut(QKeySequence::Paste);
  m_btnPaste->setToolTip(tr("Colar"));
  m_btnPaste->setEnabled(!bReadOnly);
  m_btnPaste->setVisible(!bReadOnly);

  m_btnZoomIn = new QPushButton;
  m_btnZoomIn->setFlat(true);
  m_btnZoomIn->setIconSize(QSize(24, 24));
  m_btnZoomIn->setIcon(QIcon(":/icons/res/zoomin.png"));
  m_btnZoomIn->setShortcut(QKeySequence::ZoomIn);
  m_btnZoomIn->setToolTip(tr("Zoom in"));

  m_btnZoomOut = new QPushButton;
  m_btnZoomOut->setFlat(true);
  m_btnZoomOut->setIconSize(QSize(24, 24));
  m_btnZoomOut->setIcon(QIcon(":/icons/res/zoomout.png"));
  m_btnZoomOut->setShortcut(QKeySequence::ZoomOut);
  m_btnZoomOut->setToolTip(tr("Zoom out"));

  m_btnNormalSize = new QPushButton;
  m_btnNormalSize->setFlat(true);
  m_btnNormalSize->setIconSize(QSize(24, 24));
  m_btnNormalSize->setIcon(QIcon(":/icons/res/collapse.png"));
  m_btnNormalSize->setToolTip(tr("Normal"));

  m_btnFit = new QPushButton;
  m_btnFit->setFlat(true);
  m_btnFit->setIconSize(QSize(24, 24));
  m_btnFit->setIcon(QIcon(":/icons/res/fit.png"));
  m_btnFit->setToolTip(tr("Expandir"));
  m_btnFit->setCheckable(true);

  connect(m_btnOpen, SIGNAL(clicked(bool)), this, SLOT(open()));
  connect(m_btnCopy, SIGNAL(clicked(bool)), this, SLOT(copy()));
  connect(m_btnPaste, SIGNAL(clicked(bool)), this, SLOT(paste()));
  connect(m_btnZoomIn, SIGNAL(clicked(bool)), this, SLOT(zoomIn()));
  connect(m_btnZoomOut, SIGNAL(clicked(bool)), this, SLOT(zoomOut()));
  connect(m_btnNormalSize, SIGNAL(clicked(bool)), this, SLOT(normalSize()));
  connect(m_btnFit, SIGNAL(clicked(bool)), this, SLOT(fitToWindow()));

  QHBoxLayout* ltButtons = new QHBoxLayout;
  ltButtons->setContentsMargins(0, 0, 0, 0);
  ltButtons->setAlignment(Qt::AlignLeft);
  ltButtons->addWidget(m_btnOpen);
  ltButtons->addWidget(m_btnCopy);
  ltButtons->addWidget(m_btnPaste);
  ltButtons->addWidget(m_btnZoomIn);
  ltButtons->addWidget(m_btnZoomOut);
  ltButtons->addWidget(m_btnNormalSize);
  ltButtons->addWidget(m_btnFit);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addLayout(ltButtons);
  lt->addWidget(m_scrollArea);
  setLayout(lt);
  m_btnFit->setChecked(true);
  fitToWindow();
}

bool ImageViewer::loadFile(const QString& fileName)
{
  QImageReader reader(fileName);
  reader.setAutoTransform(true);
  const QImage newImage = reader.read();
  if (newImage.isNull())
  {
    QMessageBox::information(this,
                             QGuiApplication::applicationDisplayName(),
                             tr("Erro ao carregar a imagem %1: %2")
                             .arg(QDir::toNativeSeparators(fileName),
                                  reader.errorString()));
    return false;
  }

  setImage(newImage);
  return true;
}

void ImageViewer::setImage(const QImage& image)
{
  m_image = image;
  m_imageLabel->setPixmap(QPixmap::fromImage(image));
  m_scaleFactor = 1.0;

  m_scrollArea->setVisible(!image.isNull());
  m_btnFit->setEnabled(true);
  updateActions();

  if (!m_btnFit->isChecked())
    m_imageLabel->adjustSize();
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
  static bool firstDialog = true;

  if (firstDialog)
  {
    firstDialog = false;
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
  }

  QStringList mimeTypeFilters;
  const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
    ? QImageReader::supportedMimeTypes()
    : QImageWriter::supportedMimeTypes();
  for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
  mimeTypeFilters.sort();
  dialog.setMimeTypeFilters(mimeTypeFilters);
  dialog.selectMimeTypeFilter("image/jpeg");
  if (acceptMode == QFileDialog::AcceptSave)
    dialog.setDefaultSuffix("jpg");
}

void ImageViewer::open()
{
  QFileDialog dialog(this, tr("Abrir arquivo"));
  initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

  while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(m_image);
#endif
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
  if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
    if (mimeData->hasImage()) {
      const QImage image = qvariant_cast<QImage>(mimeData->imageData());
      if (!image.isNull())
        return image;
    }
  }
  return QImage();
}
#endif

void ImageViewer::paste()
{
#ifndef QT_NO_CLIPBOARD
  const QImage newImage = clipboardImage();
  if (!newImage.isNull())
    setImage(newImage);
#endif
}

void ImageViewer::zoomIn()
{
  scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
  scaleImage(0.8);
}

void ImageViewer::normalSize()
{
  m_imageLabel->adjustSize();
  m_scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
  bool bFit = m_btnFit->isChecked();
  m_scrollArea->setWidgetResizable(bFit);
  if (!bFit)
    normalSize();
  updateActions();
}

void ImageViewer::updateActions()
{
  m_btnCopy->setEnabled(!m_image.isNull());
  m_btnZoomIn->setEnabled(!m_btnFit->isChecked());
  m_btnZoomOut->setEnabled(!m_btnFit->isChecked());
  m_btnNormalSize->setEnabled(!m_btnFit->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
  if (!m_imageLabel->pixmap())
    return;

  m_scaleFactor *= factor;
  m_imageLabel->resize(m_scaleFactor * m_imageLabel->pixmap()->size());

  adjustScrollBar(m_scrollArea->horizontalScrollBar(), factor);
  adjustScrollBar(m_scrollArea->verticalScrollBar(), factor);

  m_btnZoomIn->setEnabled(m_scaleFactor < 3.0);
  m_btnZoomOut->setEnabled(m_scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
  scrollBar->setValue(int(factor * scrollBar->value()
                      + ((factor - 1) * scrollBar->pageStep()/2)));
}

QByteArray ImageViewer::getCompressedImageAsByteArray() const
{
  QByteArray bArray;
  if (!m_image.isNull())
  {
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    if (m_imageLabel->pixmap()->width() > 640)
      m_imageLabel->pixmap()->scaledToWidth(640).save(&buffer, "PNG");
    else
      m_imageLabel->pixmap()->save(&buffer, "PNG");
  }
  return bArray;
}
