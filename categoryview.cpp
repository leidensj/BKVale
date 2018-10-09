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

  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME,
                                      tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"),
                                      true,
                                      true,
                                      false);

  QFormLayout* namelayout = new QFormLayout;
  namelayout->setContentsMargins(0, 0, 0, 0);
  namelayout->addRow(tr("Nome:"), m_edName);

  QVBoxLayout* tablayout = new QVBoxLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addLayout(namelayout);
  tablayout->addWidget(m_imagePicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  m_tab->addTab(tabframe,
                QIcon(":/icons/res/category.png"),
                tr("Categoria"));
}

void CategoryView::create()
{
  selectItem(Category());
  m_edName->setFocus();
}

const JItem& CategoryView::getItem() const
{
  static Category o;
  o.m_id = m_currentId;
  o.m_image.m_id = m_imagePicker->getId();
  o.m_name = m_edName->text();
  return o;
}

void CategoryView::setItem(const JItem& o)
{
  auto ref = static_cast<const Category&>(o);
  m_edName->setText(ref.m_name);
  m_imagePicker->setItem(ref.m_image);
}
