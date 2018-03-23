#include "personpageview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include "QDateEdit"
#include "address.h"
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>

PersonPageView::PersonPageView(QWidget* parent)
 : QFrame(parent)
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
 , m_grpBirthDate(nullptr)
 , m_dtBirthDate(nullptr)
 , m_imagePicker(nullptr)
 , m_cbCustomer(nullptr)
 , m_cbSupplier(nullptr)
 , m_cbEmployee(nullptr)
 , m_dtCreationDate(nullptr)
 , m_edPinCode(nullptr)
{
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
  m_imagePicker = new JDatabasePicker(tr("Imagem"),
                                      QIcon(":/icons/res/icon.png"),
                                      true, false, true);
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
  {
    QFont f = m_edPinCode->font();
    f.setBold(true);
    m_edPinCode->setFont(f);
  }

  QGroupBox* personGroupBox = new QGroupBox;
  personGroupBox->setTitle(tr("Tipo de pessoa"));
  QHBoxLayout* personLayout = new QHBoxLayout;
  personLayout->addWidget(m_rdoPerson);
  personLayout->addWidget(m_rdoCompany);
  personGroupBox->setLayout(personLayout);

  QFormLayout* flayout0 = new QFormLayout;
  flayout0->setContentsMargins(0, 0, 0, 0);
  flayout0->addRow(tr("Data de criação:"), m_dtCreationDate);
  flayout0->addRow(m_lblName, m_edName);
  flayout0->addRow(m_lblAlias, m_edAlias);
  flayout0->addRow(tr("Email:"), m_edEmail);
  flayout0->addRow(m_lblCpfCnpj, m_edCpfCnpj);
  flayout0->addRow(m_lblRgIE, m_edRgIE);
  flayout0->addRow(tr("Detalhes:"), m_edDetails);

  m_grpBirthDate = new QGroupBox;
  m_grpBirthDate->setCheckable(true);
  m_grpBirthDate->setChecked(false);
  m_grpBirthDate->setTitle(tr("Data de nascimento"));
  QVBoxLayout* dateLayout = new QVBoxLayout();
  dateLayout->addWidget(m_dtBirthDate);
  m_grpBirthDate->setLayout(dateLayout);

  QGroupBox* typeGroupBox = new QGroupBox;
  typeGroupBox->setTitle(tr("Disponível como"));
  QVBoxLayout* typeLayout = new QVBoxLayout();
  typeLayout->addWidget(m_cbCustomer);
  typeLayout->addWidget(m_cbSupplier);
  QHBoxLayout* employeeLayout = new QHBoxLayout;
  employeeLayout->setContentsMargins(0, 0, 0, 0);
  employeeLayout->addWidget(m_cbEmployee);
  employeeLayout->addWidget(m_edPinCode);
  typeLayout->addLayout(employeeLayout);
  typeGroupBox->setLayout(typeLayout);

  QVBoxLayout* vlayout0 = new QVBoxLayout;
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addWidget(personGroupBox);
  vlayout0->addLayout(flayout0);
  vlayout0->addWidget(m_grpBirthDate);
  vlayout0->addWidget(m_imagePicker);
  vlayout0->addWidget(typeGroupBox);
  setLayout(vlayout0);

  QObject::connect(m_rdoCompany,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(switchUserType()));
  QObject::connect(m_rdoPerson,
                   SIGNAL(toggled(bool)),
                   this,
                   SLOT(switchUserType()));
  QObject::connect(m_edName,
                   SIGNAL(textChanged(QString)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edAlias,
                   SIGNAL(textChanged(QString)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edEmail,
                   SIGNAL(textChanged(QString)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edCpfCnpj,
                   SIGNAL(textChanged(QString)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edRgIE,
                   SIGNAL(textChanged(QString)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_edDetails,
                   SIGNAL(textChanged(QString)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_dtBirthDate,
                   SIGNAL(dateChanged(const QDate&)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbCustomer,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbSupplier,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_cbEmployee,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));

  switchUserType();
  updateControls();
  m_edName->setFocus();
}

void PersonPageView::setDatabase(QSqlDatabase db)
{
  m_imagePicker->setDatabase(db, IMAGE_SQL_TABLE_NAME);
}

Person PersonPageView::getPerson() const
{
  Person person;
  person.m_id = m_currentPerson.m_id;
  person.m_image.m_id = m_imagePicker->getId();
  person.m_name = m_edName->text();
  person.m_alias = m_edAlias->text();
  person.m_email = m_edEmail->text();
  person.m_CPF_CNPJ = m_edCpfCnpj->text();
  person.m_RG_IE = m_edRgIE->text();
  person.m_details = m_edDetails->text();
  person.m_birthDate = m_grpBirthDate->isChecked() && !m_rdoCompany->isChecked()
                       ? m_dtBirthDate->date().toString(Qt::ISODate)
                       : "";
  person.m_creationDate = m_dtCreationDate->date().toString(Qt::ISODate);
  person.m_bCompany = m_rdoCompany->isChecked();
  person.m_bCustomer = m_cbCustomer->isChecked();
  person.m_bSupplier = m_cbSupplier->isChecked();
  person.m_bEmployee = m_cbEmployee->isChecked();
  return person;

}

void PersonPageView::setPerson(const Person& person)
{
  m_currentPerson = person;
  m_imagePicker->setItem(person.m_image.m_id, person.m_image.m_name, person.m_image.m_image);
  m_edName->setText(person.m_name);
  m_edAlias->setText(person.m_alias);
  m_edEmail->setText(person.m_email);
  m_edCpfCnpj->setText(person.m_CPF_CNPJ);
  m_edRgIE->setText(person.m_RG_IE);
  m_edDetails->setText(person.m_details);
  m_grpBirthDate->setChecked(!person.m_birthDate.isEmpty());
  if (m_grpBirthDate->isChecked())
    m_dtBirthDate->setDate(QDate::fromString(person.m_birthDate, Qt::ISODate));
  else
    m_dtBirthDate->setDate(QDate::currentDate());
  m_dtCreationDate->setDate(QDate::fromString(person.m_creationDate, Qt::ISODate));
  m_rdoCompany->setChecked(person.m_bCompany);
  m_cbCustomer->setChecked(person.m_bCustomer);
  m_cbSupplier->setChecked(person.m_bSupplier);
  m_cbEmployee->setChecked(person.m_bEmployee);
  switchUserType();
}

void PersonPageView::clear()
{
  m_currentPerson.clear();
  setPerson(m_currentPerson);
}

void PersonPageView::switchUserType()
{
  if (m_rdoCompany->isChecked())
  {
    m_lblName->setText(tr("Razão social:"));
    m_lblAlias->setText(tr("Nome fantasia:"));
    m_lblCpfCnpj->setText(tr("CNPJ:"));
    m_lblRgIE->setText(tr("IE:"));
    m_edCpfCnpj->setInputMask("99.999.999/9999-99;_");
    m_edRgIE->setInputMask("");
    m_cbEmployee->setChecked(false);
    m_edPinCode->setEnabled(false);
    m_grpBirthDate->hide();
  }
  else
  {
    m_lblName->setText(tr("Nome:"));
    m_lblAlias->setText(tr("Apelido:"));
    m_lblCpfCnpj->setText(tr("CPF:"));
    m_lblRgIE->setText(tr("RG:"));
    m_edCpfCnpj->setInputMask("999.999.999-99;_");
    m_edRgIE->setInputMask("9999999999;_");
    m_cbEmployee->setEnabled(true);
    m_grpBirthDate->show();
  }
  emit changedSignal();
}

void PersonPageView::updateControls()
{
  m_edPinCode->setEnabled(m_cbEmployee->isChecked());
  emit changedSignal();
}
