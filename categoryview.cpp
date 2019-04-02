#include "categoryview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

CategoryView::CategoryView(QWidget* parent)
  : JItemView(CATEGORY_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_imagePicker(nullptr)
{
  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(CATEGORY_MAX_NAME_LENGTH);

  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME);

  QFormLayout* ltForm = new QFormLayout;
  ltForm->addRow(tr("Nome:"), m_edName);
  ltForm->addRow(tr("Imagem:"), m_imagePicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltForm);

  m_tab->addTab(tabframe,
                QIcon(":/icons/res/category.png"),
                tr("Categoria"));
}

void CategoryView::create()
{
  selectItem(Category());
  m_edName->setFocus();
}

const JItemSQL& CategoryView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_image.m_id = m_imagePicker->getId();
  m_ref.m_name = m_edName->text();
  return m_ref;
}

void CategoryView::setItem(const JItemSQL& o)
{
  auto ref = static_cast<const Category&>(o);
  m_currentId = o.m_id;
  m_edName->setText(ref.m_name);
  m_imagePicker->setItem(ref.m_image);
}
