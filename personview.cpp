#include "personview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QDateEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>
#include "jdatabase.h"
#include "databaseutils.h"
#include <QAction>
#include <QGroupBox>
#include "phonetablewidget.h"
#include "addresstablewidget.h"

PersonView::PersonView(bool bAccessEmployee,
                       bool bAccessSupplier,
                       QWidget* parent)
  : JItemView(PERSON_SQL_TABLE_NAME, parent)
  , m_rdoPerson(nullptr)
  , m_rdoCompany(nullptr)
  , m_edName(nullptr)
  , m_lblName(nullptr)
  , m_edAlias(nullptr)
  , m_lblAlias(nullptr)
  , m_edEmail(nullptr)
  , m_edCpfCnpj(nullptr)
  , m_lblCpfCnpj(nullptr)
  , m_edRgIE(nullptr)
  , m_lblRgIE(nullptr)
  , m_edDetails(nullptr)
  , m_dtBirthDate(nullptr)
  , m_cbBirthDate(nullptr)
  , m_imagePicker(nullptr)
  , m_dtCreationDate(nullptr)
  , m_edPinCode(nullptr)
  , m_tblPhone(nullptr)
  , m_btnPhoneAdd(nullptr)
  , m_btnPhoneRemove(nullptr)
  , m_tblAddress(nullptr)
  , m_btnAddressAdd(nullptr)
  , m_btnAddressRemove(nullptr)
  , m_cbNoteEdit(nullptr)
  , m_cbNoteRemove(nullptr)
  , m_cbEmployee(nullptr)
  , m_cbSupplier(nullptr)
  , m_bHasAccessToEmployees(bAccessEmployee)
  , m_bHasAccessToSuppliers(bAccessSupplier)
{
  m_rdoPerson = new QRadioButton;
  m_rdoPerson->setText(tr("Fisíca"));
  m_rdoPerson->setChecked(true);
  m_rdoCompany = new QRadioButton;
  m_rdoCompany->setText(tr("Jurídica"));
  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                           JLineEdit::st_defaultFlags1);
  m_lblName = new QLabel;
  m_edAlias = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                            JLineEdit::st_defaultFlags1);
  m_lblAlias = new QLabel;
  m_edEmail = new JLineEdit(JLineEdit::Input::All,
                            JLineEdit::st_defaultFlags2);
  m_edCpfCnpj = new JLineEdit(JLineEdit::Input::Numeric,
                              JLineEdit::st_defaultFlags2);
  m_lblCpfCnpj = new QLabel;
  m_edRgIE= new JLineEdit(JLineEdit::Input::Numeric,
                          JLineEdit::st_defaultFlags2);
  m_lblRgIE = new QLabel;
  m_edDetails = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                              JLineEdit::st_defaultFlags1);
  m_dtBirthDate = new QDateEdit;
  m_dtBirthDate->setCalendarPopup(true);
  m_dtBirthDate->setDisplayFormat("dd/MM/yyyy");
  m_dtBirthDate->setDate(QDate::currentDate());
  m_dtBirthDate->setSpecialValueText(tr("Não informada"));
  m_cbBirthDate = new QCheckBox;
  m_cbBirthDate->setChecked(false);
  m_cbBirthDate->setText(tr("Nascimento:"));

  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME,
                                      tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"));
  m_dtCreationDate = new QDateEdit;
  m_dtCreationDate->setCalendarPopup(true);
  m_dtCreationDate->setDisplayFormat("dd/MM/yyyy");
  m_dtCreationDate->setDate(QDate::currentDate());
  m_dtCreationDate->setReadOnly(true);
  m_edPinCode = new JLineEdit(JLineEdit::Input::Numeric,
                              JLineEdit::st_defaultFlags2);
  m_edPinCode->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);
  m_edPinCode->setAlignment(Qt::AlignCenter);
  {
    QFont f = m_edPinCode->font();
    f.setBold(true);
    m_edPinCode->setFont(f);
  }

  m_cbNoteEdit = new QCheckBox;
  m_cbNoteEdit->setText(tr("Criar/Editar"));
  m_cbNoteEdit->setIcon(QIcon(":/icons/res/file.png"));

  m_cbNoteRemove = new QCheckBox;
  m_cbNoteRemove->setText(tr("Remover"));
  m_cbNoteRemove->setIcon(QIcon(":/icons/res/remove.png"));

  m_tblPhone = new PhoneTableWidget;
  m_btnPhoneAdd = new QPushButton(QIcon(":/icons/res/additem.png"), "");
  m_btnPhoneAdd->setFlat(true);
  m_btnPhoneAdd->setIconSize(QSize(24, 24));
  m_btnPhoneRemove = new QPushButton(QIcon(":/icons/res/removeitem.png"), "");
  m_btnPhoneRemove->setFlat(true);
  m_btnPhoneRemove->setIconSize(QSize(24, 24));

  m_tblAddress = new AddressTableWidget;
  m_btnAddressAdd = new QPushButton;
  m_btnAddressAdd->setFlat(true);
  m_btnAddressAdd->setIconSize(QSize(24, 24));
  m_btnAddressAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAddressAdd->setToolTip(tr("Adicionar endereço"));
  m_btnAddressRemove = new QPushButton;
  m_btnAddressRemove->setFlat(true);
  m_btnAddressRemove->setIconSize(QSize(24, 24));
  m_btnAddressRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnAddressRemove->setToolTip(tr("Remover endereço"));

  m_cbEmployee = new QCheckBox;
  m_cbEmployee->setIcon(QIcon(":/icons/res/employee.png"));
  m_cbEmployee->setEnabled(m_bHasAccessToEmployees);
  m_cbEmployee->setText(tr("Funcionário"));

  m_cbSupplier = new QCheckBox;
  m_cbSupplier->setIcon(QIcon(":/icons/res/supplier.png"));
  m_cbSupplier->setEnabled(m_bHasAccessToSuppliers);
  m_cbSupplier->setText(tr("Fornecedor"));

  QFormLayout* formLayout = new QFormLayout;
  formLayout->addRow(tr("Tipo:"), m_rdoPerson);
  formLayout->addRow("", m_rdoCompany);
  formLayout->addRow(tr("Data de criação:"), m_dtCreationDate);
  formLayout->addRow(m_lblName, m_edName);
  formLayout->addRow(m_lblAlias, m_edAlias);
  formLayout->addRow(tr("Email:"), m_edEmail);
  formLayout->addRow(m_lblCpfCnpj, m_edCpfCnpj);
  formLayout->addRow(m_lblRgIE, m_edRgIE);
  formLayout->addRow(tr("Detalhes:"), m_edDetails);
  formLayout->addRow(m_cbBirthDate, m_dtBirthDate);
  formLayout->addRow(tr("Imagem:"), m_imagePicker);
  formLayout->addRow(tr(""), m_cbEmployee);
  formLayout->addRow(tr(""), m_cbSupplier);

  QVBoxLayout* ltPhoneBtn = new QVBoxLayout;
  ltPhoneBtn->setContentsMargins(0, 0, 0, 0);
  ltPhoneBtn->setAlignment(Qt::AlignTop);
  ltPhoneBtn->addWidget(m_btnPhoneAdd);
  ltPhoneBtn->addWidget(m_btnPhoneRemove);

  QHBoxLayout* ltPhone = new QHBoxLayout;
  ltPhone->addWidget(m_tblPhone);
  ltPhone->addLayout(ltPhoneBtn);

  QVBoxLayout* ltAddressBtn = new QVBoxLayout;
  ltAddressBtn->setContentsMargins(0, 0, 0, 0);
  ltAddressBtn->setAlignment(Qt::AlignTop);
  ltAddressBtn->addWidget(m_btnAddressAdd);
  ltAddressBtn->addWidget(m_btnAddressRemove);

  QHBoxLayout* ltAddress = new QHBoxLayout;
  ltAddress->addWidget(m_tblAddress);
  ltAddress->addLayout(ltAddressBtn);

  QFormLayout* employeeLayout = new QFormLayout;
  employeeLayout->setAlignment(Qt::AlignTop);
  employeeLayout->addRow(tr("Pincode:"), m_edPinCode);
  employeeLayout->addRow(tr("Vales:"), m_cbNoteEdit);
  employeeLayout->addRow("", m_cbNoteRemove);

  QFrame* informationFrame = new QFrame;
  informationFrame->setLayout(formLayout);

  QFrame* employeeFrame = new QFrame;
  employeeFrame->setLayout(employeeLayout);

  QFrame* phoneFrame = new QFrame;
  phoneFrame->setLayout(ltPhone);

  QFrame* addressFrame = new QFrame;
  addressFrame->setLayout(ltAddress);

  m_tab->addTab(informationFrame, QIcon(":/icons/res/details.png"), tr("Informações"));
  m_tab->addTab(phoneFrame, QIcon(":/icons/res/phone.png"), tr("Telefone"));
  m_tab->addTab(addressFrame, QIcon(":/icons/res/address.png"), tr("Endereço"));
  m_tab->addTab(employeeFrame, QIcon(":/icons/res/employee.png"), tr("Funcionário"));

  connect(m_rdoCompany, SIGNAL(toggled(bool)), this, SLOT(switchUserType()));
  connect(m_rdoPerson, SIGNAL(toggled(bool)), this, SLOT(switchUserType()));
  connect(m_cbEmployee, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_cbSupplier, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_cbBirthDate, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_btnPhoneAdd, SIGNAL(clicked(bool)), m_tblPhone, SLOT(addItem()));
  connect(m_btnPhoneRemove, SIGNAL(clicked(bool)), m_tblPhone, SLOT(removeItem()));
  connect(m_tblPhone, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_btnAddressAdd, SIGNAL(clicked(bool)), m_tblAddress, SLOT(addItem()));
  connect(m_btnAddressRemove, SIGNAL(clicked(bool)), m_tblAddress, SLOT(removeItem()));
  connect(m_tblAddress, SIGNAL(changedSignal()), this, SLOT(updateControls()));

  switchUserType();
  m_edName->setFocus();
}

PersonView::~PersonView()
{

}

const JItem& PersonView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_image.m_id = m_imagePicker->getId();
  m_ref.m_name = m_edName->text();
  m_ref.m_alias = m_edAlias->text();
  m_ref.m_email = m_edEmail->text();
  m_ref.m_CPF_CNPJ = m_edCpfCnpj->text();
  m_ref.m_RG_IE = m_edRgIE->text();
  m_ref.m_details = m_edDetails->text();
  m_ref.m_bBirth = m_cbBirthDate->isChecked() && !m_rdoCompany->isChecked();
  m_ref.m_dtBirth = m_dtBirthDate->date();
  m_ref.m_dtCreation = m_dtCreationDate->date();
  m_ref.m_bCompany = m_rdoCompany->isChecked();

  m_ref.m_supplier.m_bIsSupplier = m_cbSupplier->isChecked();
  m_ref.m_employee.m_bIsEmployee = m_cbEmployee->isChecked();
  m_ref.m_employee.m_pincode = m_cbEmployee->isChecked() ? m_edPinCode->text() : "";
  m_ref.m_employee.m_bNoteEdit = m_cbEmployee->isChecked() && m_cbNoteEdit->isChecked();
  m_ref.m_employee.m_bNoteRemove = m_cbEmployee->isChecked() && m_cbNoteRemove->isChecked();

  for (int i = 0; i != m_tblPhone->rowCount(); ++i)
    m_ref.m_vPhone.push_back(dynamic_cast<const Phone&>(m_tblPhone->getItem(i)));

  for (int i = 0; i != m_tblAddress->rowCount(); ++i)
    m_ref.m_vAddress.push_back(dynamic_cast<const Address&>(m_tblAddress->getItem(i)));

  return m_ref;
}

void PersonView::setItem(const JItem &o)
{
  auto _o = dynamic_cast<const Person&>(o);
  m_currentId = _o.m_id;
  m_imagePicker->setItem(_o.m_image);
  m_edName->setText(_o.m_name);
  m_edAlias->setText(_o.m_alias);
  m_edEmail->setText(_o.m_email);
  m_edCpfCnpj->setText(_o.m_CPF_CNPJ);
  m_edRgIE->setText(_o.m_RG_IE);
  m_edDetails->setText(_o.m_details);
  m_cbBirthDate->setChecked(_o.m_bBirth && !_o.m_bCompany);
  m_dtBirthDate->setDate(_o.m_dtBirth);
  m_dtCreationDate->setDate(_o.m_dtCreation);
  m_rdoCompany->setChecked(_o.m_bCompany);
  m_rdoPerson->setChecked(!_o.m_bCompany);
  m_cbSupplier->setChecked(_o.m_supplier.m_bIsSupplier);
  m_cbEmployee->setChecked(_o.m_employee.m_bIsEmployee);
  switchUserType();

  m_edPinCode->setText(_o.m_employee.m_pincode);
  m_cbNoteEdit->setChecked(_o.m_employee.m_bNoteEdit);
  m_cbNoteRemove->setChecked(_o.m_employee.m_bNoteRemove);

  m_tblPhone->removeAllItems();
  m_tblAddress->removeAllItems();

  for (int i = 0; i != _o.m_vPhone.size(); ++i)
    m_tblPhone->addItem(_o.m_vPhone.at(i));

  for (int i = 0; i != _o.m_vAddress.size(); ++i)
    m_tblAddress->addItem(_o.m_vAddress.at(i));
}

void PersonView::create()
{
  selectItem(Person());
  m_tab->setCurrentIndex(0);
  updateControls();
  m_edName->setFocus();
}

void PersonView::updateControls()
{
  m_edPinCode->setEnabled(m_cbEmployee->isChecked());
  m_dtBirthDate->setEnabled(m_cbBirthDate->isChecked());
  if (m_cbBirthDate->isChecked() &&
      m_dtBirthDate->date() == m_dtBirthDate->minimumDate())
    m_dtBirthDate->setDate(QDate::currentDate());

  m_btnPhoneRemove->setEnabled(m_tblPhone->isValidCurrentRow());
  m_btnAddressRemove->setEnabled(m_tblAddress->isValidCurrentRow());
  m_tab->setTabEnabled(3, m_bHasAccessToEmployees && m_cbEmployee->isChecked());
}

void PersonView::switchUserType()
{
  if (m_rdoCompany->isChecked())
  {
    m_lblName->setText(tr("Razão social:"));
    m_lblAlias->setText(tr("Nome fantasia:"));
    m_lblCpfCnpj->setText(tr("CNPJ:"));
    m_lblRgIE->setText(tr("IE:"));
    m_edCpfCnpj->clear();
    m_edCpfCnpj->setInputMask("99.999.999/9999-99;_");
    m_edRgIE->clear();
    m_edRgIE->setInputMask("");
  }
  else
  {
    m_lblName->setText(tr("Nome:"));
    m_lblAlias->setText(tr("Apelido:"));
    m_lblCpfCnpj->setText(tr("CPF:"));
    m_lblRgIE->setText(tr("RG:"));
    m_edCpfCnpj->clear();
    m_edCpfCnpj->setInputMask("999.999.999-99;_");
    m_edRgIE->clear();
    m_edRgIE->setInputMask("9999999999;_");
  }
  updateControls();
}
