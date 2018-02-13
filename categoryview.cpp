#include "categoryview.h"
#include "jlineedit.h"
#include <QPushButton>
#include <QLayout>

CategoryView::CategoryView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_CATEGORY_ID)
  , m_currentImageId(INVALID_IMAGE_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_btnSearchImage(nullptr)
  , m_edImageName(nullptr)
  , m_btnClearImage(nullptr)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton();
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edName->setPlaceholderText(tr("Nome"));
  m_edName->setMaxLength(MAX_CATEGORY_NAME_LENGTH);

  m_btnSearchImage = new QPushButton();
  m_btnSearchImage->setFlat(true);
  m_btnSearchImage->setText("");
  m_btnSearchImage->setIconSize(QSize(16, 16));
  m_btnSearchImage->setIcon(QIcon(":/icons/res/binoculars.png"));
  m_btnSearchImage->setToolTip(tr("Buscar imagem"));

  m_edImageName = new JLineEdit(JValidatorType::All, true, true);
  m_edImageName->setReadOnly(true);
  m_edImageName->setPlaceholderText(tr("Imagem"));

  m_btnClearImage = new QPushButton();
  m_btnClearImage->setFlat(true);
  m_btnClearImage->setText("");
  m_btnClearImage->setIconSize(QSize(16, 16));
  m_btnClearImage->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClearImage->setToolTip(tr("Limpar imagem"));

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->addWidget(m_btnCreate);
  hlayout0->addWidget(m_btnSave);
  hlayout0->setAlignment(Qt::AlignLeft);

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->addWidget(m_btnSearchImage);
  hlayout1->addWidget(m_edImageName);
  hlayout1->addWidget(m_btnClearImage);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addLayout(hlayout0);
  vlayout0->addWidget(m_edName);
  vlayout0->addLayout(hlayout1);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));

  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSaveSignal()));

  QObject::connect(m_edName,
                   SIGNAL(textChanged(const QString&)),
                   this,
                   SLOT(updateControls()));

  QObject::connect(m_btnSearchImage,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(emitSearchImageSignal()));

  setLayout(vlayout0);
  updateControls();
}

void CategoryView::setImage(int imageId, const QString& imageName)
{
  m_currentImageId = imageId;
  m_edImageName->setText(imageName);
}

void CategoryView::create()
{
  m_currentId = INVALID_CATEGORY_ID;
  m_currentImageId = INVALID_IMAGE_ID;
  m_edName->setText("");
  updateControls();
}

Category CategoryView::getCategory() const
{
  Category category;
  category.m_id = m_currentId;
  category.m_name = m_edName->text();
  category.m_imageId = INVALID_IMAGE_ID;
  return category;
}

void CategoryView::setCategory(const Category &category,
                               const QString& imageName)
{
  m_currentId = category.m_id;
  m_currentImageId = category.m_imageId;
  m_edName->setText(category.m_name);
  setImage(category.m_imageId, imageName);
  updateControls();
}

void CategoryView::emitSaveSignal()
{
  emit saveSignal();
}

void CategoryView::emitSearchImageSignal()
{
  emit searchImageSignal();
}

void CategoryView::updateControls()
{
  bool bEnable = !m_edName->text().isEmpty();
  m_btnSave->setEnabled(bEnable);
  QString saveIcon = Category::st_isValidId(m_currentId)
                     ? ":/icons/res/saveas.png"
                     : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(saveIcon));
}
