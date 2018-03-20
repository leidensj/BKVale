#include "jdatabasepicker.h"
#include "jlineedit.h"
#include "jimageview.h"
#include "defines.h"
#include "jdatabase.h"
#include "databaseutils.h"
#include <QSqlTableModel>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QMessageBox>

JDatabasePicker::JDatabasePicker(const QString& text,
                                 const QIcon& icon,
                                 bool bShowImage,
                                 bool bRequired,
                                 bool bDisplayGroup,
                                 QWidget* parent)
 : QFrame(parent)
 , m_text(text)
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

  m_edText = new JLineEdit(JValidatorType::All, true, true);
  m_edText->setReadOnly(true);
  if (!bDisplayGroup)
    m_edText->setPlaceholderText(m_text);
  if (bRequired)
  {
    QString str = !bDisplayGroup ? " *" : "*";
    m_edText->setPlaceholderText(m_edText->placeholderText() + str);
  }

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
                   SIGNAL(itemSelectedSignal(qlonglong)),
                   this,
                   SLOT(searchItem(qlonglong)));

  if (!bShowImage)
  {
    m_imageView->hide();
    vFrame1->hide();
  }


}

void JDatabasePicker::setDatabase(QSqlDatabase db,
                                  const QString& tableName,
                                  const QString& filter)
{
  m_db = db;
  m_tableName = tableName;
  QSqlTableModel* model = new QSqlTableModel(this, db);
  QVector<JTableColumn> vColumns;
  if (tableName == IMAGE_SQL_TABLE_NAME)
    vColumns = Image::getColumns();
  else if (tableName == PERSON_SQL_TABLE_NAME)
    vColumns = Person::getColumns();
  else if (tableName == CATEGORY_SQL_TABLE_NAME)
    vColumns = Category::getColumns();
  else if (tableName == PRODUCT_SQL_TABLE_NAME)
    vColumns = Product::getColumns();

  m_selector->set(model, tableName, vColumns);
}

void JDatabasePicker::searchItem(qlonglong id)
{
  bool bSuccess = false;
  QString error;
  if (m_tableName == IMAGE_SQL_TABLE_NAME)
  {
    Image o;
    o.m_id = id;
    bSuccess = ImageSQL::select(m_db, o, error);
    if (bSuccess)
      setItem(o.m_id, o.m_name, o.m_image);
  }
  else if (m_tableName == PERSON_SQL_TABLE_NAME)
  {
    Person o;
    o.m_id = id;
    bSuccess = PersonSQL::select(m_db, o, error);
    if (bSuccess)
      setItem(o.m_id, o.m_alias, o.m_image.m_image);
  }
  else if (m_tableName == CATEGORY_SQL_TABLE_NAME)
  {
    Category o;
    o.m_id = id;
    bSuccess = CategorySQL::select(m_db, o, error);
    if (bSuccess)
      setItem(o.m_id, o.m_name, o.m_image.m_image);
  }
  else if (m_tableName == PRODUCT_SQL_TABLE_NAME)
  {
    Product o;
    o.m_id = id;
    bSuccess = ProductSQL::select(m_db, o, error);
    if (bSuccess)
      setItem(o.m_id, o.m_name, o.m_image.m_image);
  }

  if (!bSuccess)
  {
    clear();
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("O seguinte erro ocorreu ao selecionar o item com ID "
                             "'%1':\n'%2'").arg(QString::number(id), error));
  }


}

void JDatabasePicker::setItem(qlonglong id,
                              const QString& name,
                              const QByteArray& arImage)
{
  m_id = id;
  m_edText->setText(name);
  m_imageView->setImage(arImage);
}

void JDatabasePicker::searchItem()
{
  m_selector->exec();
}

void JDatabasePicker::clear()
{
  m_id = INVALID_ID;
  m_edText->clear();
  m_imageView->clearImage();
}

qlonglong JDatabasePicker::getId() const
{
  return m_id;
}
