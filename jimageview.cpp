#include "jimageview.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QBuffer>

JImageView::JImageView(QWidget* parent)
  : QFrame(parent)
  , m_bHasImage(false)
  , m_bDirty(false)
  , m_btnOpen(nullptr)
  , m_btnClear(nullptr)
  , m_lblImage(nullptr)
{
  m_btnOpen = new QPushButton();
  m_btnOpen->setFlat(true);
  m_btnOpen->setText("");
  m_btnOpen->setIconSize(QSize(16, 16));
  m_btnOpen->setIcon(QIcon(":/icons/res/open.png"));
  m_btnOpen->setToolTip(tr("Abrir imagem"));

  m_btnClear = new QPushButton();
  m_btnClear->setFlat(true);
  m_btnClear->setText("");
  m_btnClear->setIconSize(QSize(16, 16));
  m_btnClear->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClear->setToolTip(tr("Apagar imagem"));

  m_lblImage = new QLabel();
  m_lblImage->setMinimumSize(64, 64);
  m_lblImage->setMaximumSize(64, 64);
  m_lblImage->setScaledContents(true);

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnOpen);
  hlayout0->addWidget(m_btnClear);

  QFrame* frame0 = new QFrame();
  frame0->setFrameShape(QFrame::Shape::StyledPanel);
  frame0->setFrameShadow(QFrame::Shadow::Plain);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->addWidget(m_lblImage);
  frame0->setLayout(hlayout1);
  frame0->setMinimumSize(66, 66);
  frame0->setMaximumSize(66, 66);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->addWidget(frame0);
  hlayout2->addStretch();

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addLayout(hlayout0);
  vlayout0->addLayout(hlayout2);
  vlayout0->addStretch();
  setLayout(vlayout0);

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(openImage()));

  QObject::connect(m_btnClear,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clearImage()));

  clearImage();
  m_bDirty = false;
  updateControls();
}

void JImageView::setImage(const QString& fileName)
{
  m_bDirty = true;
  m_bHasImage = true;
  m_lblImage->setPixmap(QIcon(fileName).pixmap(QSize(64, 64)));
  updateControls();
  emit changedSignal();
}

void JImageView::setImage(const QByteArray& bArray)
{
  m_bDirty = false;
  m_bHasImage = true;
  QPixmap pixmap(QSize(64, 64));
  pixmap.loadFromData(bArray);
  m_lblImage->setPixmap(pixmap);
  updateControls();
}

QString JImageView::getImagePath()
{
  return QFileDialog::getOpenFileName(this,
                                      tr("Abrir Imagem"),
                                      "/home",
                                      tr("Imagens (*.png)"));

}

void JImageView::openImage()
{
  QString fileName = getImagePath();
  if (fileName.isEmpty())
    return;
  QFile file(fileName);
  if (file.size() > MAX_IMAGE_SIZE)
  {
    QMessageBox::warning(this,
                         tr("Aviso"),
                         tr("Arquivo de imagem muito grande. "
                            "Por favor, selecione um arquivo de "
                            "no máximo 1MB"),
                         QMessageBox::Ok);
  }
  else
  {
    setImage(fileName);
  }
}

QByteArray JImageView::getImage()
{
  QByteArray bArray;
  if (hasImage())
  {
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    m_lblImage->pixmap()->save(&buffer, "PNG");
  }
  return bArray;
}

void JImageView::clearImage()
{
  m_bDirty = true;
  m_bHasImage = false;
  m_lblImage->clear();
  m_lblImage->setPixmap(QIcon(":/icons/res/noimage.png").pixmap(QSize(64, 64)));
  updateControls();
  emit changedSignal();
}

bool JImageView::hasImage() const
{
  return m_bHasImage;
}

bool JImageView::isDirty() const
{
  return m_bDirty;
}

void JImageView::updateControls()
{
  m_btnClear->setEnabled(hasImage());
}
