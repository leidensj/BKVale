#include "jdatabasepicker.h"
#include "jlineedit.h"
#include "jimageview.h"
#include "defines.h"
#include "jdatabase.h"
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QMessageBox>

JDatabasePicker::JDatabasePicker(const QString& text,
                                 const QIcon& icon,
                                 bool bShowImage,
                                 bool bDisplayGroup,
                                 QWidget* parent)
 : QFrame(parent)
 , m_text(text)
 , m_id(INVALID_ID)
 , m_selector(nullptr)
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
  m_btnSearch->setToolTip(tr("Selecionar ") + m_text);

  m_edText = new JLineEdit(JLineEdit::Input::All,
                           JLineEdit::st_defaultFlags1);
  m_edText->setReadOnly(true);

  if (!bDisplayGroup)
    m_edText->setPlaceholderText(m_text);

  m_btnClear = new QPushButton();
  m_btnClear->setFlat(true);
  m_btnClear->setText("");
  m_btnClear->setIconSize(QSize(16, 16));
  m_btnClear->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClear->setToolTip(tr("Remover ") + m_text);

  m_imageView = new JImageView(false, 24);

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout0 = new QHBoxLayout();

  QGroupBox* group = nullptr;
  if (bDisplayGroup)
  {
    group = new QGroupBox();
    group->setTitle(m_text);
    group->setLayout(hlayout0);
  }

  hlayout0->addWidget(m_btnSearch);
  hlayout0->addWidget(m_edText);
  hlayout0->addWidget(m_btnClear);
  hlayout0->addWidget(vFrame1);
  hlayout0->addWidget(m_imageView);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setContentsMargins(0, 0, 0, 0);
  vlayout0->setAlignment(Qt::AlignTop);
  if (bDisplayGroup)
    vlayout0->addWidget(group);
  else
    vlayout0->addLayout(hlayout0);
  setLayout(vlayout0);

  m_selector = new JDatabaseSelector(tr("Selecionar ") + m_text, icon, this);

  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(searchItem()));

  QObject::connect(m_selector,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(setItem(const JItem&)));

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

void JDatabasePicker::setDatabase(QSqlDatabase db,
                                  const QString& tableName)
{
  m_selector->setDatabase(db, tableName);
}

JDatabase* JDatabasePicker::getDatabase() const
{
  return m_selector->getDatabase();
}

void JDatabasePicker::setItem(const JItem& jItem)
{
  m_selector->hide();
  QString tableName = m_selector->getDatabase()->getTableName();
  if (tableName == IMAGE_SQL_TABLE_NAME)
  {
    const Image& o = dynamic_cast<const Image&>(jItem);
    setItem(o.m_id, o.m_name, o.m_image);
  }
  else if (tableName == PERSON_SQL_TABLE_NAME)
  {
    const Person& o = dynamic_cast<const Person&>(jItem);
    setItem(o.m_id, o.m_alias, o.m_image.m_image);
  }
  else if (tableName == CATEGORY_SQL_TABLE_NAME)
  {
    const Category& o = dynamic_cast<const Category&>(jItem);
    setItem(o.m_id, o.m_name, o.m_image.m_image);
  }
  else if (tableName == PRODUCT_SQL_TABLE_NAME)
  {
    const Product& o = dynamic_cast<const Product&>(jItem);
    setItem(o.m_id, o.m_name, o.m_image.m_image);
  }
}

void JDatabasePicker::setItem(qlonglong id,
                              const QString& name,
                              const QByteArray& arImage)
{
  qlonglong previousId = m_id;
  m_id = id;
  m_edText->setText(name);
  m_imageView->setImage(arImage);
  if (previousId != m_id)
    emit changedSignal();
}

void JDatabasePicker::searchItem()
{
  m_selector->exec();
}

void JDatabasePicker::clear()
{
  qlonglong previousId = m_id;
  m_id = INVALID_ID;
  m_edText->clear();
  m_imageView->clearImage();
  if (previousId != m_id)
    emit changedSignal();
}

qlonglong JDatabasePicker::getId() const
{
  return m_id;
}
