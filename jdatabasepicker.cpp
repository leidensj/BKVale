#include "jdatabasepicker.h"
#include "jlineedit.h"
#include "jimageview.h"
#include "defines.h"
#include "jdatabase.h"
#include "store.h"
#include "product.h"
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QLabel>
#include <QAction>

JDatabasePicker::JDatabasePicker(const QString& tableName,
                                 const QString& text,
                                 const QIcon& icon,
                                 bool bMultiPicker,
                                 QWidget* parent)
 : QFrame(parent)
 , m_text(text)
 , m_bMultiPicker(bMultiPicker)
 , m_selector(nullptr)
 , m_edText(nullptr)
 , m_imageView(nullptr)
{
  m_edText = new JLineEdit(JLineEdit::Input::All, JLineEdit::st_defaultFlags1);
  m_edText->setReadOnly(true);
  QAction* action = m_edText->addAction(QIcon(":/icons/res/binoculars.png"), QLineEdit::LeadingPosition);
  QAction* action2 = m_edText->addAction(QIcon(":/icons/res/remove.png"), QLineEdit::TrailingPosition);

  m_selector = new JDatabaseSelector(tableName, tr("Selecionar ") + m_text, icon, bMultiPicker, this);

  m_imageView = new JImageView(false, 24);
  m_imageView->hide();

  QHBoxLayout* hlayout0 = new QHBoxLayout;
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->addWidget(m_edText);
  hlayout0->addWidget(m_imageView);

  setLayout(hlayout0);

  connect(action, SIGNAL(triggered(bool)), this, SLOT(searchItem()));
  connect(action2, SIGNAL(triggered(bool)), this, SLOT(clear()));
  connect(m_edText, SIGNAL(deleteSignal()), this, SLOT(clear()));
  connect(m_edText, SIGNAL(enterSignal()), this, SLOT(searchItem()));
  connect(m_selector->getDatabase(), SIGNAL(itemsSelectedSignal(const QVector<JItemSQL*>&)), this, SLOT(setItems(const QVector<JItemSQL*>&)));
}

JDatabase* JDatabasePicker::getDatabase() const
{
  return m_selector->getDatabase();
}

void JDatabasePicker::setItems(const QVector<JItemSQL*>& v)
{
  m_selector->close();
  clear();
  bool bChanged = false;
  for (int i = 0; i != v.size(); ++i)
  {
    if (v.at(i) == nullptr)
      continue;
    if (setItem(*v.at(i)))
      bChanged = true;
  }
  if (bChanged)
    emit changedSignal();
}

bool JDatabasePicker::setItem(const JItemSQL& o)
{
  QString tableName = m_selector->getDatabase()->getTableName();
  if (tableName == IMAGE_SQL_TABLE_NAME)
  {
    const Image& _o = dynamic_cast<const Image&>(o);
    return setItem(_o.m_id, _o.m_name, _o.m_image);
  }
  else if (tableName == PERSON_SQL_TABLE_NAME)
  {
    const Person& _o = dynamic_cast<const Person&>(o);
    return setItem(_o.m_id, _o.strAliasName(), _o.m_image.m_image);
  }
  else if (tableName == CATEGORY_SQL_TABLE_NAME)
  {
    const Category& _o = dynamic_cast<const Category&>(o);
    return setItem(_o.m_id, _o.m_name, _o.m_image.m_image);
  }
  else if (tableName == PRODUCT_SQL_TABLE_NAME)
  {
    const Product& _o = dynamic_cast<const Product&>(o);
    return setItem(_o.m_id, _o.m_name, _o.m_image.m_image);
  }
  else if (tableName == STORE_SQL_TABLE_NAME)
  {
    const Store& _o = dynamic_cast<const Store&>(o);
    return setItem(_o.m_id, _o.m_name, _o.m_person.m_image.m_image);
  }
  return false;
}

bool JDatabasePicker::setItem(Id id, const QString& name, const QByteArray& arImage)
{
  if (m_bMultiPicker)
  {
    bool bFound = false;
    if (id.isValid())
    {
      for (int i = 0; i != m_ids.size(); ++i)
      {
        if (m_ids.at(i) == id)
        {
          bFound = true;
          break;
        }
      }

      if (!bFound)
      {
        m_ids.push_back(id);
        m_names.push_back(name);
        QString str, strTooltip;
        for (int i = 0; i != m_names.size(); ++i)
        {
          str += m_names.at(i) + "; ";
          strTooltip += m_names.at(i) + "\n";
        }
        m_edText->setText(str);
        m_edText->setToolTip(strTooltip);
      }
    }
    return !bFound;
  }
  else
  {
    Id previousId;
    previousId.set(m_ids.isEmpty() ? INVALID_ID : m_ids.at(0).get());
    m_ids.clear();
    m_names.clear();
    m_ids.push_back(id);
    m_edText->setText(name);
    m_imageView->setImage(arImage);
    m_imageView->hasImage() ? m_imageView->show() : m_imageView->hide();
    return previousId != id;
  }
}

void JDatabasePicker::searchItem()
{
  m_selector->getDatabase()->refresh();
  if (m_bMultiPicker && !m_ids.isEmpty())
    m_selector->getDatabase()->selectIds(m_ids);
  m_selector->exec();
}

void JDatabasePicker::clear()
{
  bool bChanged = !m_edText->text().isEmpty();
  m_edText->clear();
  m_ids.clear();
  m_names.clear();
  if (bChanged)
    emit changedSignal();
}

Id JDatabasePicker::getId() const
{
  return Id(m_bMultiPicker ? INVALID_ID : m_ids.isEmpty() ? INVALID_ID : m_ids.at(0));
}

const QVector<Id>& JDatabasePicker::getIds() const
{
  return m_ids;
}

void JDatabasePicker::setPlaceholderText(bool bSet)
{
  m_edText->setPlaceholderText(bSet ? m_text : "");
}
