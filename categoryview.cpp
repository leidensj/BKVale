#include "categoryview.h"
#include "jlineedit.h"
#include "jpicker.h"
#include <QPushButton>
#include <QLayout>

CategoryView::CategoryView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_CATEGORY_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_imagePicker(nullptr)
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

  m_imagePicker = new JPicker(INVALID_IMAGE_ID, tr("Imagem"), true);

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->addWidget(m_btnCreate);
  hlayout0->addWidget(m_btnSave);
  hlayout0->setAlignment(Qt::AlignLeft);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addLayout(hlayout0);
  vlayout0->addWidget(m_edName);
  vlayout0->addWidget(m_imagePicker);

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

  QObject::connect(m_imagePicker,
                   SIGNAL(searchSignal()),
                   this,
                   SLOT(emitSearchImageSignal()));

  setLayout(vlayout0);
  updateControls();
}

void CategoryView::setImage(int id, const QString& name, const QByteArray& ar)
{
  m_imagePicker->setId(id);
  m_imagePicker->setText(name);
  m_imagePicker->setImage(ar);
}

void CategoryView::create()
{
  m_currentId = INVALID_CATEGORY_ID;
  m_edName->clear();
  m_imagePicker->clear();
  updateControls();
  m_edName->setFocus();
}

Category CategoryView::getCategory() const
{
  Category category;
  category.m_id = m_currentId;
  category.m_imageId = m_imagePicker->getId();
  category.m_name = m_edName->text();
  return category;
}

void CategoryView::setCategory(const Category &category,
                               const QString& imageName,
                               const QByteArray& arImage)
{
  m_currentId = category.m_id;
  m_edName->setText(category.m_name);
  setImage(category.m_imageId, imageName, arImage);
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
