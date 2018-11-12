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
#include <QSpinBox>
#include <QListWidget>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QByteArray>
#include <QDomDocument>
#include <QMessageBox>
#include "jdatabase.h"
#include "databaseutils.h"
#include <QAction>
#include <QGroupBox>

// Serviço de busca de cep em:
// http://postmon.com.br/
// Exemplo
/*
 * <result>
 *    <complemento>até 1020/1021</complemento>
 *    <bairro>Nossa Senhora de Lourdes</bairro>
 *    <cidade>Caxias do Sul</cidade>
 *    <logradouro>Rua Sinimbu</logradouro>
 *    <estado_info>
 *         <area_km2>281.737,947</area_km2>
 *         <codigo_ibge>43</codigo_ibge>
 *         <nome>Rio Grande do Sul</nome>
 *    </estado_info>
 *    <cep>95020000</cep>
 *    <cidade_info>
 *       <area_km2>1652,308</area_km2>
 *       <codigo_ibge>4305108</codigo_ibge>
 *    </cidade_info>
 *    <estado>RS</estado>
 * </result>
*/

namespace
{
  QString searchPostalCode(const QString& postalCode)
  {
    QString url("http://api.postmon.com.br/v1/cep/" + postalCode + "?format=xml");
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QByteArray bts = reply->readAll();
    QString str(bts);

    delete reply;
    return str;
  }
}

PersonView::PersonView(bool bAccessEmployee,
                       bool bAccessSupplier,
                       QWidget* parent)
  : JItemView(PERSON_SQL_TABLE_NAME, parent)
  , m_addressPage(nullptr)
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
  , m_spnPhoneCountryCode(nullptr)
  , m_spnPhoneCode(nullptr)
  , m_edPhoneNumber(nullptr)
  , m_edPhoneName(nullptr)
  , m_btnAddPhone(nullptr)
  , m_btnRemovePhone(nullptr)
  , m_lstPhone(nullptr)
  , m_edAddressPostalCode(nullptr)
  , m_btnAddressPostalCode(nullptr)
  , m_edAddressNeighborhood(nullptr)
  , m_edAddressStreet(nullptr)
  , m_spnAddressNumber(nullptr)
  , m_edAddressCity(nullptr)
  , m_cbAddressState(nullptr)
  , m_edAddressComplement(nullptr)
  , m_edAddressReference(nullptr)
  , m_btnAddAddress(nullptr)
  , m_btnRemoveAddress(nullptr)
  , m_lstAddress(nullptr)
  , m_cbNoteEdit(nullptr)
  , m_cbNoteRemove(nullptr)
  , m_grpEmployee(nullptr)
  , m_grpSupplier(nullptr)
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
  m_dtBirthDate->setMinimumDate(QDate(1800, 1, 1));
  m_dtBirthDate->setDate(QDate(1800, 1, 1));
  m_dtBirthDate->setSpecialValueText(tr("Não informada"));
  m_cbBirthDate = new QCheckBox;
  m_cbBirthDate->setChecked(false);
  m_cbBirthDate->setText(tr("Data de nascimento:"));

  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME,
                                      tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"),
                                      JDatabasePicker::Flags::TextGroup);
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

  m_edPhoneNumber = new JLineEdit(JLineEdit::Input::Numeric,
                                  JLineEdit::st_defaultFlags2);
  m_edPhoneName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                JLineEdit::st_defaultFlags1);
  m_edPhoneName->setPlaceholderText(tr("Nome (opcional)"));
  m_edPhoneNumber->setPlaceholderText(tr("*"));
  m_spnPhoneCountryCode = new QSpinBox;
  m_spnPhoneCountryCode->setMinimum(0);
  m_spnPhoneCountryCode->setMaximum(999999);
  m_spnPhoneCountryCode->setPrefix("+");
  m_spnPhoneCountryCode->setValue(55);
  m_spnPhoneCode = new QSpinBox;
  m_spnPhoneCode->setMinimum(0);
  m_spnPhoneCode->setMaximum(999999);
  m_spnPhoneCode->setPrefix("(");
  m_spnPhoneCode->setValue(54);
  m_spnPhoneCode->setSuffix(")");
  m_btnAddPhone = new QPushButton;
  m_btnAddPhone->setFlat(true);
  m_btnAddPhone->setIconSize(QSize(16, 16));
  m_btnAddPhone->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAddPhone->setToolTip(tr("Adicionar telefone"));
  m_btnRemovePhone = new QPushButton;
  m_btnRemovePhone->setFlat(true);
  m_btnRemovePhone->setIconSize(QSize(16, 16));
  m_btnRemovePhone->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemovePhone->setToolTip(tr("Remover telefone"));
  m_lstPhone = new QListWidget;

  m_edAddressPostalCode = new JLineEdit(JLineEdit::Input::Numeric,
                                        JLineEdit::st_defaultFlags2);
  m_edAddressPostalCode->setInputMask(ADDRESS_CEP_MASK);
  m_btnAddressPostalCode = new QPushButton();
  m_btnAddressPostalCode->setFlat(true);
  m_btnAddressPostalCode->setIconSize(QSize(16, 16));
  m_btnAddressPostalCode->setIcon(QIcon(":/icons/res/process.png"));
  m_btnAddressPostalCode->setToolTip(tr("Buscar CEP"));
  m_edAddressNeighborhood = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                          JLineEdit::st_defaultFlags1);
  m_edAddressNeighborhood->setPlaceholderText(tr("*"));
  m_edAddressStreet = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                    JLineEdit::st_defaultFlags1);
  m_edAddressStreet->setPlaceholderText(tr("*"));
  m_spnAddressNumber = new QSpinBox();
  m_spnAddressNumber->setMinimum(0);
  m_spnAddressNumber->setMaximum(999999);
  m_spnAddressNumber->setPrefix(tr("Nº "));
  m_edAddressCity = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                  JLineEdit::st_defaultFlags1);
  m_edAddressCity->setPlaceholderText(tr("*"));
  m_cbAddressState = new QComboBox();
  for (int i = 0; i != ADDRESS_NUMBER_OF_BRAZILIAN_STATES; ++i)
    m_cbAddressState->addItem(Address::st_getBRState((Address::EBRState)i).m_name);
  m_cbAddressState->setCurrentIndex((int)Address::EBRState::RS);
  m_edAddressComplement = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                        JLineEdit::st_defaultFlags1);
  m_edAddressReference = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                       JLineEdit::st_defaultFlags1);
  m_btnAddAddress = new QPushButton();
  m_btnAddAddress->setFlat(true);
  m_btnAddAddress->setIconSize(QSize(16, 16));
  m_btnAddAddress->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAddAddress->setToolTip(tr("Adicionar endereço"));
  m_btnRemoveAddress = new QPushButton();
  m_btnRemoveAddress->setFlat(true);
  m_btnRemoveAddress->setIconSize(QSize(16, 16));
  m_btnRemoveAddress->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemoveAddress->setToolTip(tr("Remover endereço"));
  m_lstAddress = new QListWidget;

  QHBoxLayout* addressButtonlayout = new QHBoxLayout();
  addressButtonlayout->setContentsMargins(0, 0, 0, 0);
  addressButtonlayout->setAlignment(Qt::AlignLeft);
  addressButtonlayout->addWidget(m_btnAddAddress);
  addressButtonlayout->addWidget(m_btnRemoveAddress);

  QHBoxLayout* addressPostalCodeLayout = new QHBoxLayout();
  addressPostalCodeLayout->setContentsMargins(0, 0, 0, 0);
  addressPostalCodeLayout->addWidget(m_edAddressPostalCode);
  addressPostalCodeLayout->addWidget(m_btnAddressPostalCode);

  QHBoxLayout* addressStreetLayout = new QHBoxLayout();
  addressStreetLayout->setContentsMargins(0, 0, 0, 0);
  addressStreetLayout->addWidget(m_edAddressStreet);
  addressStreetLayout->addWidget(m_spnAddressNumber);

  QFormLayout* addressInformationLayout = new QFormLayout();
  addressInformationLayout->setContentsMargins(0, 0, 0, 0);
  addressInformationLayout->addRow(tr("CEP:"), addressPostalCodeLayout);
  addressInformationLayout->addRow(tr("Rua:"), addressStreetLayout);
  addressInformationLayout->addRow(tr("Bairro:"), m_edAddressNeighborhood);
  addressInformationLayout->addRow(tr("Cidade:"), m_edAddressCity);
  addressInformationLayout->addRow(tr("Estado:"), m_cbAddressState);
  addressInformationLayout->addRow(tr("Complemento:"), m_edAddressComplement);
  addressInformationLayout->addRow(tr("Referência:"), m_edAddressReference);

  QHBoxLayout* phoneButtonLayout = new QHBoxLayout;
  phoneButtonLayout->setContentsMargins(0, 0, 0, 0);
  phoneButtonLayout->setAlignment(Qt::AlignLeft);
  phoneButtonLayout->addWidget(m_btnAddPhone);
  phoneButtonLayout->addWidget(m_btnRemovePhone);

  QHBoxLayout* phoneNumberLayout = new QHBoxLayout;
  phoneNumberLayout->setContentsMargins(0, 0, 0, 0);
  phoneNumberLayout->addWidget(m_spnPhoneCountryCode);
  phoneNumberLayout->addWidget(m_spnPhoneCode);
  phoneNumberLayout->addWidget(m_edPhoneNumber);

  QVBoxLayout* phoneInformationLayout = new QVBoxLayout;
  phoneInformationLayout->addWidget(m_edPhoneName);
  phoneInformationLayout->addLayout(phoneNumberLayout);

  QHBoxLayout* personLayout = new QHBoxLayout;
  personLayout->setAlignment(Qt::AlignLeft);
  personLayout->addWidget(new QLabel(tr("Tipo de pessoa:")));
  personLayout->addWidget(m_rdoPerson);
  personLayout->addWidget(m_rdoCompany);

  QFormLayout* formLayout = new QFormLayout;
  formLayout->setContentsMargins(0, 0, 0, 0);
  formLayout->addRow(tr("Data de criação:"), m_dtCreationDate);
  formLayout->addRow(m_lblName, m_edName);
  formLayout->addRow(m_lblAlias, m_edAlias);
  formLayout->addRow(tr("Email:"), m_edEmail);
  formLayout->addRow(m_lblCpfCnpj, m_edCpfCnpj);
  formLayout->addRow(m_lblRgIE, m_edRgIE);
  formLayout->addRow(tr("Detalhes:"), m_edDetails);

  QHBoxLayout* dateLayout = new QHBoxLayout;
  dateLayout->setAlignment(Qt::AlignLeft);
  dateLayout->addWidget(m_cbBirthDate);
  dateLayout->addWidget(m_dtBirthDate);

  QVBoxLayout* informationLayout = new QVBoxLayout;
  informationLayout->addLayout(personLayout);
  informationLayout->addLayout(formLayout);
  informationLayout->addLayout(dateLayout);
  informationLayout->addWidget(m_imagePicker);

  QVBoxLayout* phoneLayout = new QVBoxLayout;
  phoneLayout->addLayout(phoneButtonLayout);
  phoneLayout->addLayout(phoneInformationLayout);
  phoneLayout->addWidget(m_lstPhone);

  QVBoxLayout* addressLayout = new QVBoxLayout();
  addressLayout->addLayout(addressButtonlayout);
  addressLayout->addLayout(addressInformationLayout);
  addressLayout->addWidget(m_lstAddress);

  m_grpEmployee = new QGroupBox;
  m_grpEmployee->setTitle(tr("Disponível como funcionário"));
  m_grpEmployee->setCheckable(true);
  m_grpEmployee->setChecked(false);
  m_grpEmployee->setFlat(true);

  QFormLayout* employeeLayout = new QFormLayout;
  employeeLayout->setAlignment(Qt::AlignTop);
  employeeLayout->addRow(tr("Pincode:"), m_edPinCode);
  employeeLayout->addRow(tr("Vales:"), m_cbNoteEdit);
  employeeLayout->addRow("", m_cbNoteRemove);
  m_grpEmployee->setLayout(employeeLayout);

  QVBoxLayout* employeeFrameLayout = new QVBoxLayout;
  employeeFrameLayout->addWidget(m_grpEmployee);

  m_grpSupplier = new QGroupBox;
  m_grpSupplier->setTitle(tr("Disponível como fornecedor"));
  m_grpSupplier->setCheckable(true);
  m_grpSupplier->setChecked(false);
  m_grpSupplier->setFlat(true);
  QVBoxLayout* supplierLayout = new QVBoxLayout;
  supplierLayout->setAlignment(Qt::AlignTop);
  m_grpSupplier->setLayout(supplierLayout);

  QVBoxLayout* supplierFrameLayout = new QVBoxLayout;
  supplierFrameLayout->addWidget(m_grpSupplier);

  QFrame* informationFrame = new QFrame;
  informationFrame->setLayout(informationLayout);

  QFrame* employeeFrame = new QFrame;
  employeeFrame->setLayout(employeeFrameLayout);

  QFrame* supplierFrame = new QFrame;
  supplierFrame->setLayout(supplierFrameLayout);

  QFrame* phoneFrame = new QFrame;
  phoneFrame->setLayout(phoneLayout);

  QFrame* addressFrame = new QFrame;
  addressFrame->setLayout(addressLayout);

  m_tab->addTab(informationFrame,
                QIcon(":/icons/res/details.png"),
                tr("Informações"));
  m_tab->addTab(employeeFrame,
                QIcon(":/icons/res/employee.png"),
                tr("Funcionário"));
  m_tab->setTabEnabled(1, bAccessEmployee);
  m_tab->addTab(supplierFrame,
                QIcon(":/icons/res/supplier.png"),
                tr("Fornecedor"));
  m_tab->setTabEnabled(2, bAccessSupplier);
  m_tab->addTab(phoneFrame,
                QIcon(":/icons/res/phone.png"),
                tr("Telefone"));
  m_tab->addTab(addressFrame,
                QIcon(":/icons/res/address.png"),
                tr("Endereço"));

  QObject::connect(m_rdoCompany,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(switchUserType()));
  QObject::connect(m_rdoPerson,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(switchUserType()));
  QObject::connect(m_grpEmployee,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_grpSupplier,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbBirthDate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_btnAddPhone,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(addPhone()));
  QObject::connect(m_btnRemovePhone,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removePhone()));
  QObject::connect(m_lstPhone,
                   SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                   this,
                   SLOT(openPhone()));
  QObject::connect(m_lstPhone,
                   SIGNAL(currentRowChanged(int)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_btnAddressPostalCode,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(processPostalCode()));
  QObject::connect(m_btnAddAddress,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(addAddress()));
  QObject::connect(m_btnRemoveAddress,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(removeAddress()));
  QObject::connect(m_lstAddress,
                   SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                   this,
                   SLOT(openAddress()));
  QObject::connect(m_lstAddress,
                   SIGNAL(currentRowChanged(int)),
                   this,
                   SLOT(updateControls()));

  switchUserType();
  updateControls();
  m_edName->setFocus();
}

PersonView::~PersonView()
{

}

const JItem& PersonView::getItem() const
{
  static Person o;
  o.m_id = m_currentId;
  o.m_image.m_id = m_imagePicker->getId();
  o.m_name = m_edName->text();
  o.m_alias = m_edAlias->text();
  o.m_email = m_edEmail->text();
  o.m_CPF_CNPJ = m_edCpfCnpj->text();
  o.m_RG_IE = m_edRgIE->text();
  o.m_details = m_edDetails->text();
  o.m_dtBirth = m_cbBirthDate->isChecked() && !m_rdoCompany->isChecked()
                     ? m_dtBirthDate->date() : QDate(1800, 1, 1);
  o.m_dtCreation = m_dtCreationDate->date();
  o.m_bCompany = m_rdoCompany->isChecked();

  o.m_supplier.m_bIsSupplier = m_grpSupplier->isChecked();
  o.m_employee.m_bIsEmployee = m_grpEmployee->isChecked();
  o.m_employee.m_pincode = m_grpEmployee->isChecked() ? m_edPinCode->text() : "";
  o.m_employee.m_bNoteEdit = m_grpEmployee->isChecked() && m_cbNoteEdit->isChecked();
  o.m_employee.m_bNoteRemove = m_grpEmployee->isChecked() && m_cbNoteRemove->isChecked();

  for (int i = 0; i != m_lstPhone->count(); ++i)
    o.m_vPhone.push_back(m_lstPhone->item(i)->data(Qt::UserRole).value<Phone>());

  for (int i = 0; i != m_lstAddress->count(); ++i)
    o.m_vAddress.push_back(m_lstAddress->item(i)->data(Qt::UserRole).value<Address>());

  return o;
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
  m_cbBirthDate->setChecked(_o.m_dtBirth.year() != 1);
  m_dtBirthDate->setDate(m_cbBirthDate->isChecked() ? _o.m_dtBirth : QDate(1800, 1, 1));
  m_dtCreationDate->setDate(_o.m_dtCreation);
  m_rdoCompany->setChecked(_o.m_bCompany);
  m_grpSupplier->setChecked(_o.m_supplier.m_bIsSupplier);
  m_grpEmployee->setChecked(_o.m_employee.m_bIsEmployee);
  switchUserType();

  m_edPinCode->setText(_o.m_employee.m_pincode);
  m_cbNoteEdit->setChecked(_o.m_employee.m_bNoteEdit);
  m_cbNoteRemove->setChecked(_o.m_employee.m_bNoteRemove);

  m_lstPhone->clear();
  m_lstAddress->clear();

  clearPhone();
  for (int i = 0; i != _o.m_vPhone.size(); ++i)
    addPhone(_o.m_vPhone.at(i));

  clearAddress();
  for (int i = 0; i != _o.m_vAddress.size(); ++i)
    addAddress(_o.m_vAddress.at(i));
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
  m_edPinCode->setEnabled(m_grpEmployee->isChecked());
  m_dtBirthDate->setEnabled(m_cbBirthDate->isChecked());
  if (m_cbBirthDate->isChecked() &&
      m_dtBirthDate->date() == m_dtBirthDate->minimumDate())
    m_dtBirthDate->setDate(QDate::currentDate());

  m_btnRemovePhone->setEnabled(m_lstPhone->currentRow() != -1);
  m_btnRemoveAddress->setEnabled(m_lstAddress->currentRow() != -1);
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
    m_cbBirthDate->setEnabled(false);
    m_dtBirthDate->setEnabled(false);
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
    m_cbBirthDate->setEnabled(true);
    m_dtBirthDate->setEnabled(true);
  }
  updateControls();
}

Phone PersonView::getPhone() const
{
  Phone phone;
  phone.m_code = m_spnPhoneCode->value();
  phone.m_countryCode = m_spnPhoneCountryCode->value();
  phone.m_number = m_edPhoneNumber->text();
  phone.m_name = m_edPhoneName->text();
  return phone;
}

void PersonView::addPhone()
{
  addPhone(getPhone());
  clearPhone();
}

void PersonView::addPhone(const Phone& phone)
{
  QVariant var;
  var.setValue(phone);
  QListWidgetItem* p = new QListWidgetItem;
  p->setText(phone.getFormattedPhone());
  p->setData(Qt::UserRole, var);
  m_lstPhone->addItem(p);
}

void PersonView::removePhone()
{
  QListWidgetItem* p = m_lstPhone->takeItem(m_lstPhone->currentRow());
  if (p != nullptr)
    delete p;
}

void PersonView::openPhone()
{
  QListWidgetItem* p = m_lstPhone->item(m_lstPhone->currentRow());
  if (p != nullptr)
  {
    Phone phone = p->data(Qt::UserRole).value<Phone>();
    m_spnPhoneCode->setValue(phone.m_code);
    m_spnPhoneCountryCode->setValue(phone.m_countryCode);
    m_edPhoneNumber->setText(phone.m_number);
    m_edPhoneName->setText(phone.m_name);
  }
}

void PersonView::clearPhone()
{
  m_spnPhoneCode->setValue(PHONE_DEFAULT_CODE_VALUE);
  m_spnPhoneCountryCode->setValue(PHONE_DEFAULT_COUNTRY_CODE_VALUE);
  m_edPhoneNumber->clear();
  m_edPhoneName->clear();
}

Address PersonView::getAddress() const
{
  Address address;
  address.m_cep = m_edAddressPostalCode->text();
  address.m_neighborhood = m_edAddressNeighborhood->text();
  address.m_street = m_edAddressStreet->text();
  address.m_number = m_spnAddressNumber->value();
  address.m_city = m_edAddressCity->text();
  address.m_state = (Address::EBRState)m_cbAddressState->currentIndex();
  address.m_complement = m_edAddressComplement->text();
  address.m_reference = m_edAddressReference->text();
  return address;
}

void PersonView::addAddress()
{
  addAddress(getAddress());
  clearAddress();
}

void PersonView::addAddress(const Address& address)
{
  QVariant var;
  var.setValue(address);
  QListWidgetItem* p = new QListWidgetItem;
  p->setText(address.getFormattedAddress());
  p->setData(Qt::UserRole, var);
  m_lstAddress->addItem(p);
}

void PersonView::removeAddress()
{
  QListWidgetItem* p = m_lstAddress->takeItem(m_lstAddress->currentRow());
  if (p != nullptr)
    delete p;
}

void PersonView::openAddress()
{
  QListWidgetItem* p = m_lstAddress->item(m_lstAddress->currentRow());
  if (p != nullptr)
  {
    Address address = p->data(Qt::UserRole).value<Address>();
    m_edAddressPostalCode->setText(address.m_cep);
    m_edAddressNeighborhood->setText(address.m_neighborhood);
    m_edAddressStreet->setText(address.m_street);
    m_spnAddressNumber->setValue(address.m_number);
    m_edAddressCity->setText(address.m_city);
    m_cbAddressState->setCurrentIndex((int)address.m_state);
    m_edAddressComplement->setText(address.m_complement);
    m_edAddressReference->setText(address.m_reference);
  }
}

void PersonView::clearAddress()
{
  m_edAddressPostalCode->clear();
  m_edAddressNeighborhood->clear();
  m_edAddressStreet->clear();
  m_spnAddressNumber->setValue(0);
  m_edAddressCity->clear();
  m_cbAddressState->setCurrentIndex((int)Address::EBRState::RS);
  m_edAddressComplement->clear();
  m_edAddressReference->clear();
}

void PersonView::processPostalCode()
{
  QDomDocument doc;
  bool bSuccess = doc.setContent(searchPostalCode(m_edAddressPostalCode->text()));
  if (bSuccess)
  {
    QDomElement root = doc.documentElement();
    bSuccess = root.tagName() == "result";
    if (bSuccess)
    {
      QDomNodeList nodes = root.childNodes();
      for (int i = 0; i != nodes.size(); ++i)
      {
        QString strNode = nodes.at(i).toElement().tagName();
        QString text = nodes.at(i).toElement().text().toUpper();
        if (strNode == "bairro")
          m_edAddressNeighborhood->setText(text);
        else if (strNode == "cidade")
          m_edAddressCity->setText(text);
        else if (strNode == "logradouro")
          m_edAddressStreet->setText(text);
        else if (strNode == "estado")
          m_cbAddressState->setCurrentIndex((int)Address::st_getEBRState(text));
      }
    }
  }

  if (!bSuccess)
  {
    QMessageBox::information(this,
                             tr("CEP não encontrado"),
                             tr("Verifique se o CEP '%1' informado "
                                "está correto e se há conexão com a "
                                "Internet.").arg(m_edAddressPostalCode->text()),
                             QMessageBox::Ok);
  }
}
