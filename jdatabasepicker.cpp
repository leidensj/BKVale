#include "jdatabasepicker.h"
#include "jlineedit.h"
#include "jimageview.h"
#include "defines.h"
#include "jdatabase.h"
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QListWidget>
#include <QLabel>

#define ID_PROPERTY "id"

JDatabasePicker::JDatabasePicker(const QString& tableName,
                                 const QString& text,
                                 const QIcon& icon,
                                 bool bShowImage,
                                 bool bDisplayGroup,
                                 bool bMultipicker,
                                 QWidget* parent)
 : QFrame(parent)
 , m_text(text)
 , m_bMultipicker(bMultipicker)
 , m_selector(nullptr)
 , m_btnSearch(nullptr)
 , m_edText(nullptr)
 , m_btnClear(nullptr)
 , m_imageView(nullptr)
 , m_list(nullptr)
{
  m_btnSearch = new QPushButton();
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(16, 16));
  m_btnSearch->setIcon(QIcon(":/icons/res/binoculars.png"));
  m_btnSearch->setToolTip(tr("Selecionar ") + m_text);

  if (m_bMultipicker)
  {
    m_list = new QListWidget;
  }
  else
  {
    m_edText = new JLineEdit(JLineEdit::Input::All,
                             JLineEdit::st_defaultFlags1);
    m_edText->setReadOnly(true);
  }

  if (!bDisplayGroup && !m_bMultipicker)
  {
    m_edText->setPlaceholderText(m_text);
  }

  m_btnClear = new QPushButton();
  m_btnClear->setFlat(true);
  m_btnClear->setText("");
  m_btnClear->setIconSize(QSize(16, 16));
  m_btnClear->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClear->setToolTip(tr("Remover ") + m_text);

  if (!m_bMultipicker)
    m_imageView = new JImageView(false, 24);

  QHBoxLayout* hlayout0 = new QHBoxLayout;
  QVBoxLayout* vlayout0 = new QVBoxLayout;

  QGroupBox* group = nullptr;
  if (bDisplayGroup && !m_bMultipicker)
  {
    group = new QGroupBox();
    group->setTitle(m_text);
    group->setLayout(hlayout0);
  }

  if (m_bMultipicker)
  {
    hlayout0->setContentsMargins(0, 0, 0, 0);
    hlayout0->setAlignment(Qt::AlignLeft);
    hlayout0->addWidget(m_btnSearch);
    hlayout0->addWidget(m_btnClear);
    hlayout0->addWidget(new QLabel(m_text));
    vlayout0->setContentsMargins(0, 0, 0, 0);
    vlayout0->setAlignment(Qt::AlignTop);
    vlayout0->addLayout(hlayout0);
    vlayout0->addWidget(m_list);
    m_list->setFixedHeight(70);
    vlayout0->addStretch();
  }
  else
  {
    hlayout0->addWidget(m_btnSearch);
    hlayout0->addWidget(m_edText);
    hlayout0->addWidget(m_btnClear);

    if (bShowImage)
    {
      QFrame* sep = new QFrame;
      sep->setFrameShape(QFrame::VLine);
      hlayout0->addWidget(sep);
    }
    else
    {
      m_imageView->hide();
    }
    hlayout0->addWidget(m_imageView);
    vlayout0->setContentsMargins(0, 0, 0, 0);
    vlayout0->setAlignment(Qt::AlignTop);
    if (bDisplayGroup)
      vlayout0->addWidget(group);
    else
      vlayout0->addLayout(hlayout0);
  }

  setLayout(vlayout0);

  if (m_bMultipicker)
    setFixedHeight(sizeHint().height());

  m_selector = new JDatabaseSelector(tableName, tr("Selecionar ") + m_text, icon, this);

  QObject::connect(m_btnSearch,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(searchItem()));

  QObject::connect(m_selector->getDatabase(),
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(setItem(const JItem&)));

  QObject::connect(m_btnClear,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clear()));
}

JDatabase* JDatabasePicker::getDatabase() const
{
  return m_selector->getDatabase();
}

void JDatabasePicker::setItem(const JItem& jItem)
{
  m_selector->close();
  QString tableName = m_selector->getDatabase()->getTableName();
  if (tableName == IMAGE_SQL_TABLE_NAME)
  {
    const Image& o = dynamic_cast<const Image&>(jItem);
    setItem(o.m_id, o.m_name, o.m_image);
  }
  else if (tableName == PERSON_SQL_TABLE_NAME)
  {
    const Person& o = dynamic_cast<const Person&>(jItem);
    setItem(o.m_id, o.strAliasName(), o.m_image.m_image);
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
  if (m_bMultipicker)
  {
    if (id != INVALID_ID)
    {
      bool bFound = false;
      for (int i = 0; i != m_list->count(); ++i)
      {
        if (m_list->item(i)->data(Qt::UserRole).toLongLong() == id)
        {
          bFound = true;
          break;
        }
      }

      if (!bFound)
      {
        m_list->addItem(name);
        m_list->item(m_list->count() - 1)->setData(Qt::UserRole, id);
        emit changedSignal();
      }
    }
  }
  else
  {
    qlonglong previousId = m_edText->property(ID_PROPERTY).toLongLong();
    m_edText->setProperty(ID_PROPERTY, id);
    m_edText->setText(name);
    m_imageView->setImage(arImage);
    if (previousId != id)
      emit changedSignal();
  }

}

void JDatabasePicker::searchItem()
{
  m_selector->getDatabase()->refresh();
  m_selector->exec();
}

void JDatabasePicker::clear()
{
  if (m_bMultipicker)
  {
    auto pt = m_list->takeItem(m_list->currentRow());
    if (pt != nullptr)
    {
      emit changedSignal();
      delete pt;
    }
  }
  else
  {
    m_edText->clear();
    m_imageView->clearImage();
    qlonglong previousId = m_edText->property(ID_PROPERTY).toLongLong();
    m_edText->setProperty(ID_PROPERTY, INVALID_ID);
    if (previousId != INVALID_ID)
      emit changedSignal();
  }
}

void JDatabasePicker::clearAll()
{
  if (m_bMultipicker)
  {
    bool bChanged = m_list->count() != 0;
    m_list->clear();
    if (bChanged)
      emit changedSignal();
  }
  else
  {
    clear();
  }
}

qlonglong JDatabasePicker::getId() const
{
  return m_bMultipicker ? INVALID_ID : m_edText->property(ID_PROPERTY).toLongLong();
}

QVector<qlonglong> JDatabasePicker::getIds() const
{
  QVector<qlonglong> v;
  if (m_bMultipicker)
    for (int i = 0; i != m_list->count(); ++i)
      v.push_back(m_list->item(i)->data(Qt::UserRole).toLongLong());
  return v;
}
