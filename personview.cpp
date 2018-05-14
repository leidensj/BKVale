#include "personview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QPushButton>
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
#include <QSplitter>
#include <QAction>

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

PersonView::PersonView(QWidget* parent)
  : QFrame(parent)
  , m_currentId(INVALID_ID)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
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
  , m_cbCustomer(nullptr)
  , m_cbSupplier(nullptr)
  , m_cbEmployee(nullptr)
  , m_dtCreationDate(nullptr)
  , m_edPinCode(nullptr)
  , m_spnPhoneCountryCode(nullptr)
  , m_spnPhoneCode(nullptr)
  , m_edPhoneNumber(nullptr)
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
  , m_database(nullptr)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton();
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  m_rdoPerson = new QRadioButton;
  m_rdoPerson->setText(tr("Fisíca"));
  m_rdoPerson->setIcon(QIcon(":/icons/res/house.png"));
  m_rdoPerson->setChecked(true);
  m_rdoCompany = new QRadioButton;
  m_rdoCompany->setText(tr("Jurídica"));
  m_rdoCompany->setIcon(QIcon(":/icons/res/building.png"));
  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_lblName = new QLabel;
  m_edAlias = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_lblAlias = new QLabel;
  m_edEmail = new JLineEdit(JValidatorType::All, false, true);
  m_edCpfCnpj = new JLineEdit(JValidatorType::Numeric, false, true);
  m_lblCpfCnpj = new QLabel;
  m_edRgIE= new JLineEdit(JValidatorType::Numeric, false, true);
  m_lblRgIE = new QLabel;
  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_dtBirthDate = new QDateEdit;
  m_dtBirthDate->setCalendarPopup(true);
  m_dtBirthDate->setDisplayFormat("dd/MM/yyyy");
  m_dtBirthDate->setDate(QDate::currentDate());
  m_cbBirthDate = new QCheckBox;
  m_cbBirthDate->setChecked(false);
  m_cbBirthDate->setText(tr("Data de nascimento:"));
  m_imagePicker = new JDatabasePicker(tr("Imagem"),  QIcon(":/icons/res/icon.png"), true, true);
  m_cbCustomer = new QCheckBox;
  m_cbCustomer->setText(tr("Cliente"));
  m_cbCustomer->setIcon(QIcon(":/icons/res/client.png"));
  m_cbSupplier = new QCheckBox;
  m_cbSupplier->setText(tr("Fornecedor"));
  m_cbSupplier->setIcon(QIcon(":/icons/res/supplier.png"));
  m_cbEmployee = new QCheckBox;
  m_cbEmployee->setText(tr("Funcionário"));
  m_cbEmployee->setIcon(QIcon(":/icons/res/employee.png"));
  m_dtCreationDate = new QDateEdit;
  m_dtCreationDate->setCalendarPopup(true);
  m_dtCreationDate->setDisplayFormat("dd/MM/yyyy");
  m_dtCreationDate->setDate(QDate::currentDate());
  m_dtCreationDate->setReadOnly(true);
  m_edPinCode = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edPinCode->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);
  m_edPinCode->setAlignment(Qt::AlignCenter);
  m_edPinCode->setPlaceholderText(tr("Código PIN"));
  m_edPinCode->addAction(QIcon(":/icons/res/pincode.png"), QLineEdit::LeadingPosition);
  {
    QFont f = m_edPinCode->font();
    f.setBold(true);
    m_edPinCode->setFont(f);
  }

  m_edPhoneNumber = new JLineEdit(JValidatorType::Numeric, false, true);
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

  m_edAddressPostalCode = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edAddressPostalCode->setInputMask(ADDRESS_CEP_MASK);
  m_btnAddressPostalCode = new QPushButton();
  m_btnAddressPostalCode->setFlat(true);
  m_btnAddressPostalCode->setIconSize(QSize(16, 16));
  m_btnAddressPostalCode->setIcon(QIcon(":/icons/res/process.png"));
  m_btnAddressPostalCode->setToolTip(tr("Buscar CEP"));
  m_edAddressNeighborhood = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
  m_edAddressNeighborhood->setPlaceholderText(tr("*"));
  m_edAddressStreet = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
  m_edAddressStreet->setPlaceholderText(tr("*"));
  m_spnAddressNumber = new QSpinBox();
  m_spnAddressNumber->setMinimum(0);
  m_spnAddressNumber->setMaximum(999999);
  m_spnAddressNumber->setPrefix(tr("Nº "));
  m_edAddressCity = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
  m_edAddressCity->setPlaceholderText(tr("*"));
  m_cbAddressState = new QComboBox();
  for (int i = 0; i != ADDRESS_NUMBER_OF_BRAZILIAN_STATES; ++i)
    m_cbAddressState->addItem(Address::st_getBRState((Address::EBRState)i).m_name);
  m_cbAddressState->setCurrentIndex((int)Address::EBRState::RS);
  m_edAddressComplement = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
  m_edAddressReference = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true,true);
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

  QHBoxLayout* phoneInformationLayout = new QHBoxLayout;
  phoneInformationLayout->setContentsMargins(0, 0, 0, 0);
  phoneInformationLayout->addWidget(m_spnPhoneCountryCode);
  phoneInformationLayout->addWidget(m_spnPhoneCode);
  phoneInformationLayout->addWidget(m_edPhoneNumber);

  QHBoxLayout* buttonLayout = new QHBoxLayout();
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  buttonLayout->setAlignment(Qt::AlignLeft);
  buttonLayout->addWidget(m_btnCreate);
  buttonLayout->addWidget(m_btnSave);

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

  QVBoxLayout* typeLayout = new QVBoxLayout();
  typeLayout->setAlignment(Qt::AlignTop);
  typeLayout->addWidget(m_cbCustomer);
  typeLayout->addWidget(m_cbSupplier);
  typeLayout->addWidget(m_cbEmployee);
  typeLayout->addWidget(m_edPinCode);

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

  QFrame* informationFrame = new QFrame;
  informationFrame->setLayout(informationLayout);

  QFrame* typeFrame = new QFrame;
  typeFrame->setLayout(typeLayout);

  QFrame* phoneFrame = new QFrame;
  phoneFrame->setLayout(phoneLayout);

  QFrame* addressFrame = new QFrame;
  addressFrame->setLayout(addressLayout);

  QTabWidget* tabWidget = new QTabWidget;
  tabWidget->addTab(informationFrame,
                    QIcon(":/icons/res/details.png"),
                    tr("Informações"));
  tabWidget->addTab(typeFrame,
                    QIcon(":/icons/res/check.png"),
                    tr("Disponibilidade"));
  tabWidget->addTab(phoneFrame,
                    QIcon(":/icons/res/phone.png"),
                    tr("Telefone"));
  tabWidget->addTab(addressFrame,
                    QIcon(":/icons/res/address.png"),
                    tr("Endereço"));

  m_database = new JDatabase;

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->setAlignment(Qt::AlignTop);
  viewLayout->addLayout(buttonLayout);
  viewLayout->addWidget(tabWidget);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addWidget(splitter);
  setLayout(mainlayout);

  QObject::connect(m_rdoCompany,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(switchUserType()));
  QObject::connect(m_rdoPerson,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(switchUserType()));
  QObject::connect(m_cbEmployee,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
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
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));
  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(itemSelected(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemRemovedSignal(qlonglong)),
                   this,
                   SLOT(itemRemoved(qlonglong)));

  switchUserType();
  updateControls();
  m_edName->setFocus();
}

PersonView::~PersonView()
{

}

void PersonView::setDatabase(QSqlDatabase db)
{
  m_imagePicker->setDatabase(db, IMAGE_SQL_TABLE_NAME);
  m_database->setDatabase(db, PERSON_SQL_TABLE_NAME);
}

Person PersonView::getPerson() const
{
  Person person;
  person.m_id = m_currentId;
  person.m_image.m_id = m_imagePicker->getId();
  person.m_name = m_edName->text();
  person.m_alias = m_edAlias->text();
  person.m_email = m_edEmail->text();
  person.m_CPF_CNPJ = m_edCpfCnpj->text();
  person.m_RG_IE = m_edRgIE->text();
  person.m_details = m_edDetails->text();
  person.m_birthDate = m_cbBirthDate->isChecked() && !m_rdoCompany->isChecked()
                       ? m_dtBirthDate->date().toString(Qt::ISODate)
                       : "";
  person.m_creationDate = m_dtCreationDate->date().toString(Qt::ISODate);
  person.m_bCompany = m_rdoCompany->isChecked();
  person.m_bCustomer = m_cbCustomer->isChecked();
  person.m_bSupplier = m_cbSupplier->isChecked();
  person.m_bEmployee = m_cbEmployee->isChecked();
  person.m_employeePinCode = m_edPinCode->text();

  for (int i = 0; i != m_lstPhone->count(); ++i)
    person.m_vPhone.push_back(m_lstPhone->item(i)->data(Qt::UserRole).value<Phone>());

  for (int i = 0; i != m_lstAddress->count(); ++i)
    person.m_vAddress.push_back(m_lstAddress->item(i)->data(Qt::UserRole).value<Address>());

  return person;
}

void PersonView::setPerson(const Person &person)
{
  m_currentId = person.m_id;
  m_imagePicker->setItem(person.m_image.m_id, person.m_image.m_name, person.m_image.m_image);
  m_edName->setText(person.m_name);
  m_edAlias->setText(person.m_alias);
  m_edEmail->setText(person.m_email);
  m_edCpfCnpj->setText(person.m_CPF_CNPJ);
  m_edRgIE->setText(person.m_RG_IE);
  m_edDetails->setText(person.m_details);
  m_cbBirthDate->setChecked(!person.m_birthDate.isEmpty());
  if (m_cbBirthDate->isChecked())
    m_dtBirthDate->setDate(QDate::fromString(person.m_birthDate, Qt::ISODate));
  else
    m_dtBirthDate->setDate(QDate::currentDate());
  m_dtCreationDate->setDate(QDate::fromString(person.m_creationDate, Qt::ISODate));
  m_rdoCompany->setChecked(person.m_bCompany);
  m_cbCustomer->setChecked(person.m_bCustomer);
  m_cbSupplier->setChecked(person.m_bSupplier);
  m_cbEmployee->setChecked(person.m_bEmployee);
  switchUserType();

  m_lstPhone->clear();
  m_lstAddress->clear();

  clearPhone();
  for (int i = 0; i != person.m_vPhone.size(); ++i)
    addPhone(person.m_vPhone.at(i));

  clearAddress();
  for (int i = 0; i != m_lstAddress->count(); ++i)
    addAddress(person.m_vAddress.at(i));
}

void PersonView::create()
{
  m_currentId = INVALID_ID;
  setPerson(Person());
  updateControls();
}

void PersonView::updateControls()
{
  m_btnSave->setIcon(QIcon(IS_VALID_ID(m_currentId)
                           ? ":/icons/res/saveas.png"
                           : ":/icons/res/save.png"));
  m_edPinCode->setEnabled(m_cbEmployee->isChecked());
  m_dtBirthDate->setEnabled(m_cbBirthDate->isChecked());
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
    m_cbEmployee->setChecked(false);
    m_edPinCode->setEnabled(false);
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
    m_cbEmployee->setEnabled(true);
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
  }
}

void PersonView::clearPhone()
{
  m_spnPhoneCode->setValue(PHONE_DEFAULT_CODE_VALUE);
  m_spnPhoneCountryCode->setValue(PHONE_DEFAULT_COUNTRY_CODE_VALUE);
  m_edPhoneNumber->clear();
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

void PersonView::itemSelected(const JItem& jItem)
{
  const Person& person = dynamic_cast<const Person&>(jItem);
  setPerson(person);
}

void PersonView::itemRemoved(qlonglong id)
{
  if (id == m_currentId)
    create();
}

void PersonView::save()
{
  if (m_database->save(getPerson()))
    create();
}
