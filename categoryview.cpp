#include "categoryview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jdatabase.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>
#include <QMessageBox>

CategoryView::CategoryView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
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

  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(CATEGORY_MAX_NAME_LENGTH);

  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME,
                                      tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"),
                                      true,
                                      true);

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

  QVBoxLayout* viewlayout = new QVBoxLayout;
  viewlayout->setContentsMargins(0, 0, 0, 0);
  viewlayout->setAlignment(Qt::AlignTop);
  viewlayout->addLayout(buttonlayout);
  viewlayout->addWidget(tabWidget);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewlayout);

  m_database = new JDatabase(CATEGORY_SQL_TABLE_NAME);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));
  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));
}

void CategoryView::itemSelected(const JItem& jItem)
{
  const Category& category = dynamic_cast<const Category&>(jItem);
  if (category.isValidId())
    setCategory(category);
}

void CategoryView::itemRemoved(qlonglong id)
{
  if (m_currentId == id)
    create();
}

void CategoryView::save()
{
  Category category = getCategory();
  if (m_database->save(category))
    create();
}

void CategoryView::create()
{
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_currentId = INVALID_ID;
  setCategory(Category());
  m_edName->setFocus();
}

Category CategoryView::getCategory() const
{
  Category category;
  category.m_id = m_currentId;
  category.m_image.m_id = m_imagePicker->getId();
  category.m_name = m_edName->text();
  return category;
}

void CategoryView::setCategory(const Category &category)
{
  QString strIcon = category.isValidId()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  m_currentId = category.m_id;
  m_edName->setText(category.m_name);
  m_imagePicker->setItem(category.m_image.m_id,
                         category.m_image.m_name,
                         category.m_image.m_image);
}
