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
  connect(m_selector->getDatabase(), SIGNAL(itemsSelectedSignal(const QVector<JItem*>&)), this, SLOT(setItems(const QVector<JItem*>&)));

  if (m_bMultiPicker)
    m_edText->setProperty(ID_PROPERTY, QList<QVariant>());
  else
    m_edText->setProperty(ID_PROPERTY, INVALID_ID);
}

JDatabase* JDatabasePicker::getDatabase() const
{
  return m_selector->getDatabase();
}

void JDatabasePicker::setItems(const QVector<JItem*>& items)
{
  m_selector->close();
  clear();
  bool bChanged = false;
  for (int i = 0; i != items.size(); ++i)
  {
    if (items.at(i) == nullptr)
      continue;
    if (setItem(*items.at(i)))
      bChanged = true;
  }
  if (bChanged)
    emit changedSignal();
}

bool JDatabasePicker::setItem(const JItem& jItem)
{
  QString tableName = m_selector->getDatabase()->getTableName();
  if (tableName == IMAGE_SQL_TABLE_NAME)
  {
    const Image& o = dynamic_cast<const Image&>(jItem);
    return setItem(o.m_id, o.m_name, o.m_image);
  }
  else if (tableName == PERSON_SQL_TABLE_NAME)
  {
    const Person& o = dynamic_cast<const Person&>(jItem);
    return setItem(o.m_id, o.strAliasName(), o.m_image.m_image);
  }
  else if (tableName == CATEGORY_SQL_TABLE_NAME)
  {
    const Category& o = dynamic_cast<const Category&>(jItem);
    return setItem(o.m_id, o.m_name, o.m_image.m_image);
  }
  else if (tableName == PRODUCT_SQL_TABLE_NAME)
  {
    const Product& o = dynamic_cast<const Product&>(jItem);
    return setItem(o.m_id, o.m_name, o.m_image.m_image);
  }
  else if (tableName == STORE_SQL_TABLE_NAME)
  {
    const Store& o = dynamic_cast<const Store&>(jItem);
    return setItem(o.m_id, o.m_name, o.m_person.m_image.m_image);
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
      auto lst = m_edText->property(ID_PROPERTY).toList();
      for (int i = 0; i != lst.size(); ++i)
      {
        if (lst.at(i).toLongLong() == id.get())
        {
          bFound = true;
          break;
        }
      }

      if (!bFound)
      {
        lst.push_back(id.get());
        m_edText->setProperty(ID_PROPERTY, lst);
        QString str = m_edText->text();
        if (!str.isEmpty())
          str += "; ";
        str += name;
        m_edText->setText(str);
        QString strTooltip = str.replace(';', '\n');
        m_edText->setToolTip(strTooltip);
      }
    }
    return !bFound;
  }
  else
  {
    Id previousId;
    previousId.set(m_edText->property(ID_PROPERTY).toLongLong());
    m_edText->setProperty(ID_PROPERTY, id.get());
    m_edText->setText(name);
    m_imageView->setImage(arImage);
    m_imageView->hasImage() ? m_imageView->show() : m_imageView->hide();
    return previousId != id;
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
    m_edText->setProperty(ID_PROPERTY, QList<QVariant>());
    QString str = m_edText->text();
    m_edText->clear();
    m_edText->setToolTip("");
    if (!str.isEmpty())
      emit changedSignal();
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

Id JDatabasePicker::getId() const
{
  return Id(m_bMultiPicker ? INVALID_ID : m_edText->property(ID_PROPERTY).toLongLong());
}

QVector<Id> JDatabasePicker::getIds() const
{
  QVector<Id> v;
  if (m_bMultiPicker)
  {
    auto lst = m_edText->property(ID_PROPERTY).toList();
    for (int i = 0; i != lst.size(); ++i)
      v.push_back(Id(lst.at(i).toLongLong()));
  }
  return v;
}

void JDatabasePicker::setPlaceholderText(bool bSet)
{
  m_edText->setPlaceholderText(bSet ? m_text : "");
}
