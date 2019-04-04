#include "storeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jplaintextedit.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

StoreView::StoreView(QWidget* parent)
  : JItemView(STORE_SQL_TABLE_NAME, parent)
  , m_teDescription(nullptr)
  , m_formPicker(nullptr)
  , m_addressPicker(nullptr)
  , m_phonePicker(nullptr)
{
  m_teDescription = new JPlainTextEdit;

  m_formPicker = new JDatabasePicker(FORM_SQL_TABLE_NAME);
  m_formPicker->getDatabase()->setFixedFilter(FORM_FILTER_COMPANY);

  // TODO MOSTRAR APENAS TELEFONE DA PESSOA
  m_addressPicker = new JDatabasePicker(ADDRESS_SQL_TABLE_NAME);
  m_phonePicker = new JDatabasePicker(PHONE_SQL_TABLE_NAME);

  QFormLayout* tablayout = new QFormLayout;
  tablayout->setAlignment(Qt::AlignTop);
  tablayout->addRow(tr("Cadastro:"), m_formPicker);
  tablayout->addRow(tr("Endereço:"), m_addressPicker);
  tablayout->addRow(tr("Telefone:"), m_phonePicker);
  tablayout->addRow(tr("Descrição:"), m_teDescription);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(tablayout);

  m_tab->addTab(tabframe, QIcon(":/icons/res/store.png"), tr("Loja"));
}

void StoreView::create()
{
  selectItem(Store());
}

const JItemSQL& StoreView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_form.m_id = m_formPicker->getId();
  m_ref.m_address.m_id = m_addressPicker->getId();
  m_ref.m_phone.m_id = m_phonePicker->getId();
  m_ref.m_description = m_teDescription->toPlainText();
  return m_ref;
}

void StoreView::setItem(const JItemSQL& o)
{
  const Store& ref = static_cast<const Store&>(o);
  m_currentId = o.m_id;
  m_teDescription->setPlainText(ref.m_description);
  m_formPicker->setItem(ref.m_form);
  m_addressPicker->setItem(ref.m_address);
  m_phonePicker->setItem(ref.m_phone);
}
