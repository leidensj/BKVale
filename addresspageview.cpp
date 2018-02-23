#include "addresspageview.h"
#include "jlineedit.h"
#include <QPushButton>
#include <QSpinBox>
#include <QFormLayout>
#include <QComboBox>

AddressPageView::AddressPageView(QWidget *parent)
  : QFrame(parent)
  , m_edCep(nullptr)
  , m_btnCep(nullptr)
  , m_edNeighborhood(nullptr)
  , m_edStreet(nullptr)
  , m_spnNumber(nullptr)
  , m_cbState(nullptr)
  , m_edComplement(nullptr)
  , m_edReference(nullptr)
{
  m_edCep = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edCep->setInputMask("99999-99;_");
  m_btnCep = new QPushButton();
  m_btnCep->setFlat(true);
  m_btnCep->setText("");
  m_btnCep->setIconSize(QSize(16, 16));
  m_btnCep->setIcon(QIcon(":/icons/res/process.png"));
  m_btnCep->setToolTip(tr("Buscar CEP"));
  m_edNeighborhood = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
  m_edStreet = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
  m_spnNumber = new QSpinBox();
  m_spnNumber->setMinimum(0);
  m_spnNumber->setMaximum(999999);
  m_cbState = new QComboBox();
  for (int i = 0; i != NUMBER_OF_BRAZILIAN_STATES; ++i)
    m_cbState->addItem(Address::st_BRState((Address::EBRState)i).m_name);
  m_cbState->setCurrentIndex((int)Address::EBRState::RS);
  m_edComplement = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
  m_edReference = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);

  QHBoxLayout* ceplayout = new QHBoxLayout();
  ceplayout->setContentsMargins(0, 0, 0, 0);
  ceplayout->addWidget(m_edCep);
  ceplayout->addWidget(m_btnCep);

  QHBoxLayout* streetlayout = new QHBoxLayout();
  streetlayout->setContentsMargins(0, 0, 0, 0);
  streetlayout->addWidget(m_edStreet);
  streetlayout->addWidget(m_spnNumber);

  QFormLayout* flayout0 = new QFormLayout();
  flayout0->addRow(tr("CEP:"), ceplayout);
  flayout0->addRow(tr("Rua:"), streetlayout);
  flayout0->addRow(tr("Bairro:"), m_edNeighborhood);
  flayout0->addRow(tr("Estado:"), m_cbState);
  flayout0->addRow(tr("Complemento:"), m_edComplement);
  flayout0->addRow(tr("Referência:"), m_edReference);
  setLayout(flayout0);
}
