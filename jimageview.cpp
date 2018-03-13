#include "jimageview.h"
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QBuffer>

JImageView::JImageView(bool bShowButtons, int size, QWidget* parent)
  : QFrame(parent)
  , m_size(size)
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
  m_btnOpen->setToolTip(tr("Selecionar imagem"));

  m_btnClear = new QPushButton();
  m_btnClear->setFlat(true);
  m_btnClear->setText("");
  m_btnClear->setIconSize(QSize(16, 16));
  m_btnClear->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClear->setToolTip(tr("Remover imagem"));

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnOpen);
  hlayout0->addWidget(m_btnClear);

  m_lblImage = new QLabel();
  m_lblImage->setMinimumSize(m_size, m_size);
  m_lblImage->setMaximumSize(m_size, m_size);
  m_lblImage->setScaledContents(true);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addLayout(hlayout0);
  vlayout0->addWidget(m_lblImage);
  setLayout(vlayout0);

  QObject::connect(m_btnOpen,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(openImage()));

  QObject::connect(m_btnClear,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clearImage()));

  if (!bShowButtons)
  {
    m_btnOpen->setEnabled(false);
    m_btnClear->setEnabled(false);
    m_btnOpen->hide();
    m_btnClear->hide();
  }

  m_lblImage->setFrameShape(QFrame::Shape::StyledPanel);
  m_lblImage->setFrameShadow(QFrame::Shadow::Plain);

  clearImage();
  updateControls();
}

void JImageView::setImage(const QString& fileName)
{
  m_bHasImage = true;
  m_lblImage->setPixmap(QIcon(fileName).pixmap(QSize(m_size, m_size)));
  updateControls();
  emit changedSignal();
}

void JImageView::setImage(const QByteArray& bArray)
{
  if (!bArray.isEmpty() && ! bArray.isNull())
  {
    m_bHasImage = true;
    QPixmap pixmap(QSize(m_size, m_size));
    pixmap.loadFromData(bArray);
    m_lblImage->setPixmap(pixmap);
    updateControls();
  }
}

QString JImageView::getImagePath()
{
  return QFileDialog::getOpenFileName(this,
                                      tr("Selecionar Imagem"),
                                      "/home",
                                      tr("Imagens (*.png)"));

}

void JImageView::openImage()
{
  QString fileName = getImagePath();
  if (fileName.isEmpty())
    return;
  QFile file(fileName);
  if (file.size() > IMAGE_MAX_SIZE)
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
  m_bHasImage = false;
  m_lblImage->clear();
  m_lblImage->setPixmap(QIcon(":/icons/res/noimage.png").pixmap(QSize(m_size, m_size)));
  updateControls();
  emit changedSignal();
}

bool JImageView::hasImage() const
{
  return m_bHasImage;
}

void JImageView::updateControls()
{
  m_btnClear->setEnabled(hasImage());
}
