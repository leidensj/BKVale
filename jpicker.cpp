#include "jpicker.h"
#include "jlineedit.h"
#include "jimageview.h"
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>

JPicker::JPicker(int invalidId,
                 const QString& itemText,
                 bool bShowImage,
                 bool bRequired,
                 QWidget* parent)
 : QFrame(parent)
 , m_itemText(itemText)
 , m_invalidId(invalidId)
 , m_id(m_invalidId)
 , m_btnSearch(nullptr)
 , m_edText(nullptr)
 , m_btnClear(nullptr)
 , m_imageView(nullptr)
{
  m_btnSearch = new QPushButton();
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(16, 16));
  m_btnSearch->setIcon(QIcon(":/icons/res/binoculars.png"));
  m_btnSearch->setToolTip(tr("Selecionar ") + m_itemText);

  m_edText = new JLineEdit(JValidatorType::All, true, true);
  m_edText->setReadOnly(true);
  if (bRequired)
    m_edText->setPlaceholderText("*");

  m_btnClear = new QPushButton();
  m_btnClear->setFlat(true);
  m_btnClear->setText("");
  m_btnClear->setIconSize(QSize(16, 16));
  m_btnClear->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClear->setToolTip(tr("Remover ") + m_itemText);

  m_imageView = new JImageView(false, 24);

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->addWidget(m_btnSearch);
  hlayout0->addWidget(m_edText);
  hlayout0->addWidget(m_btnClear);
  hlayout0->addWidget(vFrame1);
  hlayout0->addWidget(m_imageView);

  QGroupBox* group = new QGroupBox();
  group->setTitle(m_itemText);
  group->setLayout(hlayout0);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addWidget(group);
  setLayout(vlayout0);

  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSearchSignal()));

  QObject::connect(m_btnClear,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clear()));

  if (!bShowImage)
  {
    m_imageView->hide();
    vFrame1->hide();
  }
}

void JPicker::setId(int id)
{
  m_id = id;
}

void JPicker::setText(const QString& text)
{
  m_edText->setText(text);
}

void JPicker::setImage(const QByteArray& ar)
{
  m_imageView->setImage(ar);
}

int JPicker::getId() const
{
  return m_id;
}

QString JPicker::getText() const
{
  return m_edText->text();
}

QByteArray JPicker::getImage() const
{
  return m_imageView->getImage();
}

void JPicker::clear()
{
  m_id = m_invalidId;
  m_edText->clear();
  m_imageView->clearImage();
}

void JPicker::emitSearchSignal()
{
  emit searchSignal();
  focusNextChild();
  focusNextChild();
  focusNextChild();
}
