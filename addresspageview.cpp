#include "addresspageview.h"
#include "jlineedit.h"
#include <QPushButton>
#include <QSpinBox>
#include <QFormLayout>
#include <QComboBox>
#include <QListWidget>

namespace
{
  QString buildAbv(const Address& address)
  {
    return address.m_street + ", " +
        QString::number(address.m_number) + ". " +
        address.m_state + ".";
  }
}

AddressPageView::AddressPageView(QWidget *parent)
  : QFrame(parent)
  , m_bEditMode(false)
  , m_edCep(nullptr)
  , m_btnCep(nullptr)
  , m_edNeighborhood(nullptr)
  , m_edStreet(nullptr)
  , m_spnNumber(nullptr)
  , m_cbState(nullptr)
  , m_edComplement(nullptr)
  , m_edReference(nullptr)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
  , m_btnUndo(nullptr)
  , m_btnRemove(nullptr)
  , m_list(nullptr)
{
  m_edCep = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edCep->setInputMask("99999-99;_");
  m_btnCep = new QPushButton();
  m_btnCep->setFlat(true);
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
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setIconSize(QSize(16, 16));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setToolTip(tr("Novo"));
  m_btnSave = new QPushButton();
  m_btnSave->setFlat(true);
  m_btnSave->setIconSize(QSize(16, 16));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setToolTip(tr("Salvar"));
  m_btnUndo = new QPushButton();
  m_btnUndo->setFlat(true);
  m_btnUndo->setIconSize(QSize(16, 16));
  m_btnUndo->setIcon(QIcon(":/icons/res/undo.png"));
  m_btnUndo->setToolTip(tr("Desfazer"));
  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setIconSize(QSize(16, 16));
  m_btnRemove->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnRemove->setToolTip(tr("Remover"));
  m_list = new QListWidget();

  QHBoxLayout* buttonlayout = new QHBoxLayout();
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->setAlignment(Qt::AlignLeft);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);
  buttonlayout->addWidget(m_btnUndo);
  buttonlayout->addWidget(m_btnRemove);

  QHBoxLayout* ceplayout = new QHBoxLayout();
  ceplayout->setContentsMargins(0, 0, 0, 0);
  ceplayout->addWidget(m_edCep);
  ceplayout->addWidget(m_btnCep);

  QHBoxLayout* streetlayout = new QHBoxLayout();
  streetlayout->setContentsMargins(0, 0, 0, 0);
  streetlayout->addWidget(m_edStreet);
  streetlayout->addWidget(m_spnNumber);

  QFormLayout* flayout0 = new QFormLayout();
  flayout0->setContentsMargins(0, 0, 0, 0);
  flayout0->addRow(tr("CEP:"), ceplayout);
  flayout0->addRow(tr("Rua:"), streetlayout);
  flayout0->addRow(tr("Bairro:"), m_edNeighborhood);
  flayout0->addRow(tr("Estado:"), m_cbState);
  flayout0->addRow(tr("Complemento:"), m_edComplement);
  flayout0->addRow(tr("Referência:"), m_edReference);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->addLayout(buttonlayout);
  vlayout0->addLayout(flayout0);
  vlayout0->addWidget(m_list);
  setLayout(vlayout0);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(clearInputOnly()));
  QObject::connect(m_btnRemove,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeSelectedAddress()));
  QObject::connect(m_btnUndo,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(undo()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));
  QObject::connect(m_list,
                   SIGNAL(currentRowChanged(int)),
                   this,
                   SLOT(updateControls()));
  updateControls();
}

void AddressPageView::save()
{
  Address address = getAddress();
  saveAddress(address);
}

void AddressPageView::updateControls()
{
  m_list->setEnabled(!m_bEditMode);
  m_btnCreate->setEnabled(!m_bEditMode);
  m_btnUndo->setEnabled(m_bEditMode);
  m_btnRemove->setEnabled(!m_bEditMode && m_list->currentRow() != -1);
  QString saveIcon = m_bEditMode
                     ? ":/icons/res/saveas.png"
                     : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(saveIcon));
}

void AddressPageView::undo()
{
  if (m_bEditMode)
    saveAddress(m_currentAddress);
  updateControls();
}

void AddressPageView::saveAddress(const Address& address)
{
  QVariant var;
  var.setValue(address);
  QListWidgetItem* p = new QListWidgetItem;
  p->setText(buildAbv(address));
  p->setData(Qt::UserRole, var);
  m_list->addItem(p);
  clearInputOnly();
  updateControls();
}

void AddressPageView::openSelectedAddress()
{
  QListWidgetItem* p = m_list->item(m_list->currentRow());
  if (p != nullptr)
  {
    m_bEditMode = true;
    m_currentAddress = p->data(Qt::UserRole).value<Address>();
    setAddress(m_currentAddress);
    m_list->removeItemWidget(p);
    updateControls();
  }
}

void AddressPageView::removeSelectedAddress()
{
  QListWidgetItem* p = m_list->item(m_list->currentRow());
  if (p != nullptr)
  {
    m_list->removeItemWidget(m_list->item(m_list->currentRow()));
    updateControls();
  }
}

void AddressPageView::clear()
{
  clearInputOnly();
  m_list->clear();
  updateControls();
}

void AddressPageView::clearInputOnly()
{
  m_bEditMode = false;
  m_currentAddress = Address();
  setAddress(m_currentAddress);
  updateControls();
}

void AddressPageView::setAddress(const Address& address)
{
  m_currentAddress = address;
  m_edCep->setText(address.m_cep);
  m_edNeighborhood->setText(address.m_neighborhood);
  m_edStreet->setText(address.m_street);
  m_spnNumber->setValue(address.m_number);
  m_cbState->setCurrentIndex(m_cbState->findText(address.m_state));
  m_edComplement->setText(address.m_complement);
  m_edReference->setText(address.m_reference);
  updateControls();
}

Address AddressPageView::getAddress()
{
  Address address;
  address.m_id = m_currentAddress.m_id;
  address.m_cep = m_edCep->text();
  address.m_neighborhood = m_edNeighborhood->text();
  address.m_street = m_edStreet->text();
  address.m_number = m_spnNumber->value();
  address.m_state = Address::st_BRState((Address::EBRState)m_cbState->currentIndex()).m_abv;
  address.m_complement = m_edComplement->text();
  address.m_reference = m_edReference->text();
  return address;
}
