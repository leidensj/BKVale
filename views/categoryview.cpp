#include "categoryview.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatabasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

CategoryView::CategoryView(QWidget* parent)
  : JItemView(CATEGORY_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_imagePicker(nullptr)
{
  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(CATEGORY_MAX_NAME_LENGTH);

  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME);

  QFormLayout* ltForm = new QFormLayout;
  ltForm->addRow(tr("Nome:"), m_edName);
  ltForm->addRow(tr("Imagem:"), m_imagePicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltForm);

  m_tab->addTab(tabframe, QIcon(":/icons/res/category.png"), tr("Categoria"));
  setFocusWidgetOnCreate(m_edName);
}

void CategoryView::getItem(JItemSQL& o) const
{
  Category& _o = dynamic_cast<Category&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_image.m_id = m_imagePicker->getId();
  _o.m_name = m_edName->text();
}

void CategoryView::setItem(const JItemSQL& o)
{
  const Category& _o = static_cast<const Category&>(o);
  m_edName->setText(_o.m_name);
  m_imagePicker->setItem(_o.m_image);
}
