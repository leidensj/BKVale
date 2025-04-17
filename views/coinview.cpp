#include "coinview.h"
#include "widgets/jlineedit.h"
#include "widgets/jdoublespinbox.h".h"
#include "controls/databasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QMessageBox>

CoinView::CoinView(QWidget* parent)
  : JItemView(COIN_SQL_TABLE_NAME, parent)
  , m_edName(nullptr)
  , m_sbTax(nullptr)
  , m_imagePicker(nullptr)
{
  addViewButton(IMAGE_SQL_TABLE_NAME);

  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edName->setPlaceholderText(tr("*"));
  m_edName->setMaxLength(COIN_MAX_NAME_LENGTH);

  m_sbTax = new JDoubleSpinBox();
  m_sbTax->setMinimum(0);
  m_sbTax->setMaximum(100);

  m_imagePicker = new DatabasePicker(IMAGE_SQL_TABLE_NAME);

  QFormLayout* ltForm = new QFormLayout;
  ltForm->addRow(tr("Nome:"), m_edName);
  ltForm->addRow(tr("Taxa:"), m_sbTax);
  ltForm->addRow(tr("Imagem:"), m_imagePicker);

  QFrame* tabframe = new QFrame;
  tabframe->setLayout(ltForm);

  m_tab->addTab(tabframe, QIcon(":/icons/res/coin.png"), tr("Categoria"));
  setFocusWidgetOnClear(m_edName);
  clear();
}

void CoinView::getItem(JItemSQL& o) const
{
  Coin& _o = dynamic_cast<Coin&>(o);
  _o.clear(true);
  _o.m_id = m_id;
  _o.m_image.m_id = m_imagePicker->getFirstId();
  _o.m_name = m_edName->text();
  _o.m_tax = m_sbTax->value();
}

void CoinView::setItem(const JItemSQL& o)
{
  const Coin& _o = static_cast<const Coin&>(o);
  m_edName->setText(_o.m_name);
  m_sbTax->setValue(_o.m_tax);
  m_imagePicker->addItem(_o.m_image);
}
