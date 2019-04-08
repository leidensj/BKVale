#include "storeview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "jplaintextedit.h"
#include <QLayout>
#include <QFormLayout>
#include <qlistwidget.h>

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
  tablayout->addRow(m_formPicker->getText() + ":", m_formPicker);
  tablayout->addRow(m_addressPicker->getText() + ":", m_addressPicker);
  tablayout->addRow(m_phonePicker->getText() + ":", m_phonePicker);
  tablayout->addRow(tr("Descrição:"), m_teDescription);

  m_list = new QListWidget;
  QVBoxLayout* ltEmployee = new QVBoxLayout;
  ltEmployee->addWidget(m_list);

  QFrame* fr = new QFrame;
  fr->setLayout(tablayout);

  QFrame* frEmployee = new QFrame;
  frEmployee->setLayout(ltEmployee);

  m_tab->addTab(fr, QIcon(":/icons/res/store.png"), tr("Loja"));
  m_tab->addTab(frEmployee, QIcon(":/icons/res/employee.png"), tr("Funcionários"));

  connect(m_formPicker, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  updateControls();
}

void StoreView::create()
{
  selectItem(Store());
  updateControls();
}

void StoreView::updateControls()
{
  m_phonePicker->setEnabled(m_formPicker->getId().isValid());
  m_addressPicker->setEnabled(m_formPicker->getId().isValid());
  if (m_formPicker->getId().isValid())
  {
    m_phonePicker->getDatabase()->setFixedFilter(PHONE_FORM_FILTER +
                                                 m_formPicker->getId().str());
    m_addressPicker->getDatabase()->setFixedFilter(ADDRESS_FORM_FILTER +
                                                 m_formPicker->getId().str());
  }
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
  m_list->clear();
  m_list->addItems(ref.SQL_select_employees());
}
