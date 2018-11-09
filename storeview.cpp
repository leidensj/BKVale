#include "storeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

StoreView::StoreView(QWidget* parent)
  : JItemView(STORE_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_personPicker(nullptr)
  , m_addressPicker(nullptr)
  , m_phonePicker(nullptr)
{
  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edName->setPlaceholderText(tr("*"));

  m_personPicker = new JDatabasePicker(PERSON_SQL_TABLE_NAME,
                                       tr("Empresa"),
                                       QIcon(":/icons/res/person.png"));

  QFormLayout* tablayout = new QFormLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addRow(tr("Nome:"), m_edName);
  tablayout->addRow(tr("Empresa:"), m_personPicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  m_tab->addTab(tabframe,
                QIcon(":/icons/res/store.png"),
                tr("Loja"));
}

void StoreView::create()
{
  selectItem(Store());
  m_edName->setFocus();
}

const JItem& StoreView::getItem() const
{
  m_currentStore.m_id = m_currentId;
  m_currentStore.m_person.m_id = m_personPicker->getId();
  m_currentStore.m_name = m_edName->text();
  return m_currentStore;
}

void StoreView::setItem(const JItem& o)
{
  auto ref = static_cast<const Store&>(o);
  m_currentId = o.m_id;
  m_edName->setText(ref.m_name);
  m_personPicker->setItem(ref.m_person);
}
