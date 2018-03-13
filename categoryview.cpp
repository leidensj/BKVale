#include "categoryview.h"
#include "jlineedit.h"
#include "jpicker.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>

CategoryView::CategoryView(QWidget* parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_edName(nullptr)
  , m_imagePicker(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(CATEGORY_MAX_NAME_LENGTH);

  m_imagePicker = new JPicker(tr("Imagem"), true, false);

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);
  buttonlayout->setAlignment(Qt::AlignLeft);

  QFormLayout* namelayout = new QFormLayout;
  namelayout->setContentsMargins(0, 0, 0, 0);
  namelayout->addRow(tr("Nome:"), m_edName);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addLayout(namelayout);
  tablayout->addWidget(m_imagePicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(tabframe,
                    QIcon(":/icons/res/category.png"),
                    tr("Categoria"));

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->setContentsMargins(0, 0, 0, 0);
  mainlayout->setAlignment(Qt::AlignTop);
  mainlayout->addLayout(buttonlayout);
  mainlayout->addWidget(tabWidget);
  setLayout(mainlayout);

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

  QObject::connect(m_imagePicker,
                   SIGNAL(searchSignal()),
                   this,
                   SLOT(updateControls()));

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
  m_currentCategory = Category();
  m_edName->clear();
  m_imagePicker->clear();
  updateControls();
  m_edName->setFocus();
}

Category CategoryView::getCategory() const
{
  Category category;
  category.m_id = m_currentCategory.m_id;
  category.m_imageId = m_imagePicker->getId();
  category.m_name = m_edName->text();
  return category;
}

void CategoryView::setCategory(const FullCategory &fCategory)
{
  m_currentCategory = fCategory.m_category;
  m_edName->setText(fCategory.m_category.m_name);
  setImage(fCategory.m_image.m_id, fCategory.m_image.m_name, fCategory.m_image.m_image);
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
  Category category = getCategory();
  bool bEnable = category.isValid();
  QString saveIcon(":/icons/res/save.png");
  if (m_currentCategory.isValidId())
  {
    saveIcon = ":/icons/res/saveas.png";
    bEnable = bEnable && m_currentCategory != category;
  }
  m_btnSave->setEnabled(bEnable);
  m_btnSave->setIcon(QIcon(saveIcon));
}
