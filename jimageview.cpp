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

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addWidget(m_btnOpen);
  vlayout0->addWidget(m_btnClear);

  QFrame* frame0 = new QFrame();
  frame0->setFrameShape(QFrame::Shape::StyledPanel);
  frame0->setFrameShadow(QFrame::Shadow::Plain);

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->addWidget(m_lblImage);
  frame0->setLayout(hlayout0);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(frame0);
  hlayout1->addLayout(vlayout0);
  setLayout(hlayout1);

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(openImage()));

  QObject::connect(m_btnClear,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clearImage()));

  clearImage();
}

void JImageView::setImage(const QString& fileName)
{
  m_bHasImage = true;
  m_lblImage->setPixmap(QIcon(fileName).pixmap(QSize(64, 64)));
}

void JImageView::setImage(const QByteArray& bArray)
{
  QPixmap pixmap(QSize(64, 64));
  pixmap.loadFromData(bArray);
  m_lblImage->setPixmap(pixmap);
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
  QBuffer buffer(&bArray);
  buffer.open(QIODevice::WriteOnly);
  m_lblImage->pixmap()->save(&buffer, "PNG");
  return bArray;
}

void JImageView::clearImage()
{
  m_bHasImage = false;
  m_lblImage->clear();
  m_lblImage->setPixmap(QIcon(":/icons/res/noimage.png").pixmap(QSize(64, 64)));
}

bool JImageView::hasImage() const
{
  return m_bHasImage;
}
