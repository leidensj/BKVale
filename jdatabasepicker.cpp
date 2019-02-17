#include "jdatabasepicker.h"
#include "jlineedit.h"
#include "jimageview.h"
#include "defines.h"
#include "jdatabase.h"
#include "store.h"
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QListWidget>
#include <QLabel>
#include <QAction>

#define ID_PROPERTY "id"

JDatabasePicker::JDatabasePicker(const QString& tableName,
                                 const QString& text,
                                 const QIcon& icon,
                                 bool bMultiPicker,
                                 QWidget* parent)
 : QFrame(parent)
 , m_text(text)
 , m_bMultiPicker(bMultiPicker)
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

  m_btnClear = new QPushButton();
  m_btnClear->setFlat(true);
  m_btnClear->setText("");
  m_btnClear->setIconSize(QSize(16, 16));
  m_btnClear->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnClear->setToolTip(tr("Remover ") + m_text);

  if (m_bMultiPicker)
  {
    m_list = new QListWidget;
  }
  else
  {
    m_edText = new JLineEdit(JLineEdit::Input::All,
                             JLineEdit::st_defaultFlags1);
    m_edText->setReadOnly(true);
    m_btnSearch->hide();
    m_btnClear->hide();
    QAction* action = m_edText->addAction(QIcon(":/icons/res/binoculars.png"), QLineEdit::LeadingPosition);
    QAction* action2 = m_edText->addAction(QIcon(":/icons/res/remove.png"), QLineEdit::TrailingPosition);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(searchItem()));
    connect(action2, SIGNAL(triggered(bool)), this, SLOT(clear()));
    connect(m_edText, SIGNAL(deleteSignal()), this, SLOT(clear()));
  }

  if (!m_bMultiPicker)
    m_imageView = new JImageView(false, 24);

  QHBoxLayout* hlayout0 = new QHBoxLayout;
  QVBoxLayout* vlayout0 = new QVBoxLayout;

  if (m_bMultiPicker)
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

    m_imageView->hide();
    hlayout0->addWidget(m_imageView);
    vlayout0->setContentsMargins(0, 0, 0, 0);
    vlayout0->setAlignment(Qt::AlignTop);
    vlayout0->addLayout(hlayout0);
  }

  setLayout(vlayout0);

  if (m_bMultiPicker)
    setFixedHeight(sizeHint().height());

  m_selector = new JDatabaseSelector(tableName, tr("Selecionar ") + m_text, icon, this);

  connect(m_btnSearch, SIGNAL(clicked(bool)), this, SLOT(searchItem()));
  connect(m_selector->getDatabase(), SIGNAL(itemsSelectedSignal(const QVector<JItem*>&)), this, SLOT(setItems(const QVector<JItem*>&)));
  connect(m_btnClear, SIGNAL(clicked(bool)), this, SLOT(clear()));
  if (!m_bMultiPicker)
  {
    connect(m_edText, SIGNAL(enterSignal()), this, SLOT(searchItem()));
    connect(m_edText, SIGNAL(deleteSignal()), this, SLOT(clearAll()));
  }
}

JDatabase* JDatabasePicker::getDatabase() const
{
  return m_selector->getDatabase();
}

void JDatabasePicker::setItems(const QVector<JItem*>& items)
{
  m_selector->close();
  for (int i = 0; i != items.size(); ++i)
  {
    if (items.at(i) == nullptr)
      continue;
    setItem(*items.at(i));
  }
}

void JDatabasePicker::setItem(const JItem& jItem)
{
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
  else if (tableName == STORE_SQL_TABLE_NAME)
  {
    const Store& o = dynamic_cast<const Store&>(jItem);
    setItem(o.m_id, o.m_name, o.m_person.m_image.m_image);
  }
}

void JDatabasePicker::setItem(Id id,
                              const QString& name,
                              const QByteArray& arImage)
{
  if (m_bMultiPicker)
  {
    if (id.isValid())
    {
      bool bFound = false;
      for (int i = 0; i != m_list->count(); ++i)
      {
        if (m_list->item(i)->data(Qt::UserRole).toLongLong() == id.get())
        {
          bFound = true;
          break;
        }
      }

      if (!bFound)
      {
        m_list->addItem(name);
        m_list->item(m_list->count() - 1)->setData(Qt::UserRole, id.get());
        emit changedSignal();
      }
    }
  }
  else
  {
    Id previousId;
    previousId.set(m_edText->property(ID_PROPERTY).toLongLong());
    m_edText->setProperty(ID_PROPERTY, id.get());
    m_edText->setText(name);
    m_imageView->setImage(arImage);
    m_imageView->hasImage() ? m_imageView->show() : m_imageView->hide();
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
  if (m_bMultiPicker)
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
    Id previousId;
    previousId.set(m_edText->property(ID_PROPERTY).toLongLong());
    m_edText->setProperty(ID_PROPERTY, INVALID_ID);
    if (previousId != INVALID_ID)
      emit changedSignal();
  }
}

void JDatabasePicker::clearAll()
{
  if (m_bMultiPicker)
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

Id JDatabasePicker::getId() const
{
  return Id(m_bMultiPicker ? INVALID_ID : m_edText->property(ID_PROPERTY).toLongLong());
}

QVector<Id> JDatabasePicker::getIds() const
{
  QVector<Id> v;
  if (m_bMultiPicker)
    for (int i = 0; i != m_list->count(); ++i)
      v.push_back(Id(m_list->item(i)->data(Qt::UserRole).toLongLong()));
  return v;
}

void JDatabasePicker::setPlaceholderText(bool bSet)
{
  if (!m_bMultiPicker)
    m_edText->setPlaceholderText(bSet ? m_text : "");
}
