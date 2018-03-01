#include "personpageview.h"
#include "jlineedit.h"
#include "jpicker.h"
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
 , m_edAlias(nullptr)
 , m_edEmail(nullptr)
 , m_edCpfCnpj(nullptr)
 , m_edRgIE(nullptr)
 , m_edDetails(nullptr)
 , m_grpBirthDate(nullptr)
 , m_dtBirthDate(nullptr)
 , m_imagePicker(nullptr)
 , m_cbCustomer(nullptr)
 , m_cbSupplier(nullptr)
 , m_cbEmployee(nullptr)
 , m_dtCreationDate(nullptr)
{
  m_rdoPerson = new QRadioButton;
  m_rdoPerson->setText(tr("Fisíca"));
  m_rdoPerson->setIcon(QIcon(":/icons/res/house.png"));
  m_rdoPerson->setChecked(true);
  m_rdoCompany = new QRadioButton;
  m_rdoCompany->setText(tr("Jurídica"));
  m_rdoCompany->setIcon(QIcon(":/icons/res/building.png"));
  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edAlias = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edEmail = new JLineEdit(JValidatorType::Email, false, true);
  m_edCpfCnpj = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edRgIE= new JLineEdit(JValidatorType::Numeric, false, true);
  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_dtBirthDate = new QDateEdit;
  m_dtBirthDate->setCalendarPopup(true);
  m_dtBirthDate->setDisplayFormat("dd/MM/yyyy");
  m_dtBirthDate->setDate(QDate::currentDate());
  m_imagePicker = new JPicker(INVALID_IMAGE_ID, tr("Imagem"), true);
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

  QGroupBox* personGroupBox = new QGroupBox;
  personGroupBox->setTitle(tr("Tipo de pessoa"));
  QHBoxLayout* personLayout = new QHBoxLayout;
  personLayout->addWidget(m_rdoPerson);
  personLayout->addWidget(m_rdoCompany);
  personGroupBox->setLayout(personLayout);

  QFormLayout* flayout0 = new QFormLayout;
  flayout0->setContentsMargins(0, 0, 0, 0);
  flayout0->addRow(tr("Data de criação:"), m_dtCreationDate);
  flayout0->addRow(tr("Nome/razão social:"), m_edName);
  flayout0->addRow(tr("Apelido/nome fantasia:"), m_edAlias);
  flayout0->addRow(tr("Email:"), m_edEmail);
  flayout0->addRow(tr("CPF/CNPJ:"), m_edCpfCnpj);
  flayout0->addRow(tr("RG/IE:"), m_edRgIE);
  flayout0->addRow(tr("Detalhes:"), m_edDetails);

  m_grpBirthDate = new QGroupBox;
  m_grpBirthDate->setCheckable(true);
  m_grpBirthDate->setTitle(tr("Data de aniversário"));
  QVBoxLayout* dateLayout = new QVBoxLayout();
  dateLayout->addWidget(m_dtBirthDate);
  m_grpBirthDate->setLayout(dateLayout);

  QGroupBox* typeGroupBox = new QGroupBox;
  typeGroupBox->setTitle(tr("Disponível como"));
  QVBoxLayout* typeLayout = new QVBoxLayout();
  typeLayout->addWidget(m_cbCustomer);
  typeLayout->addWidget(m_cbSupplier);
  typeLayout->addWidget(m_cbEmployee);
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
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
  QObject::connect(m_rdoPerson,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(updateControls()));
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
  QObject::connect(m_imagePicker,
                   SIGNAL(searchSignal()),
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
  QObject::connect(m_imagePicker,
                   SIGNAL(searchSignal()),
                   this,
                   SLOT(emitSearchImageSignal()));

  updateControls();
  m_edName->setFocus();
}

Person PersonPageView::getPerson() const
{
  Person person;
  person.m_id = m_currentPerson.m_id;
  person.m_imageId = m_imagePicker->getId();
  person.m_name = m_edName->text();
  person.m_alias = m_edAlias->text();
  person.m_email = m_edEmail->text();
  person.m_CPF_CNPJ = m_edCpfCnpj->text();
  person.m_RG_IE = m_edRgIE->text();
  person.m_details = m_edDetails->text();
  person.m_birthDate = m_grpBirthDate->isChecked()
                       ? m_dtBirthDate->date().toJulianDay()
                       : INVALID_PERSON_DATE;
  person.m_creationDate = m_dtCreationDate->date().toJulianDay();
  person.m_bCompany = m_rdoCompany->isChecked();
  person.m_bCustomer = m_cbCustomer->isChecked();
  person.m_bSupplier = m_cbSupplier->isChecked();
  person.m_bEmployee = m_cbEmployee->isChecked();
  return person;

}

void PersonPageView::setPerson(const Person& person,
                               const QString& imageName,
                               const QByteArray& arImage)
{
  m_currentPerson = person;
  m_imagePicker->setId(person.m_imageId);
  m_imagePicker->setText(imageName);
  m_imagePicker->setImage(arImage);
  m_edName->setText(person.m_name);
  m_edAlias->setText(person.m_alias);
  m_edEmail->setText(person.m_email);
  m_edCpfCnpj->setText(person.m_CPF_CNPJ);
  m_edRgIE->setText(person.m_RG_IE);
  m_edDetails->setText(person.m_details);
  m_grpBirthDate->setChecked(person.m_birthDate != INVALID_PERSON_DATE);
  if (person.m_birthDate != INVALID_PERSON_DATE)
    m_dtBirthDate->setDate(QDate::fromJulianDay(person.m_birthDate));
  else
    m_dtBirthDate->setDate(QDate::currentDate());
  m_dtCreationDate->setDate(QDate::fromJulianDay(person.m_creationDate));
  m_rdoCompany->setChecked(person.m_bCompany);
  m_cbCustomer->setChecked(person.m_bCustomer);
  m_cbSupplier->setChecked(person.m_bSupplier);
  m_cbEmployee->setChecked(person.m_bEmployee);
}

void PersonPageView::setImage(int id, const QString& name, const QByteArray& ar)
{
  m_imagePicker->setId(id);
  m_imagePicker->setText(name);
  m_imagePicker->setImage(ar);
}

void PersonPageView::emitSearchImageSignal()
{
  emit searchImageSignal();
}

void PersonPageView::clear()
{
  m_currentPerson.clear();
  setPerson(m_currentPerson, "", QByteArray());
}

void PersonPageView::updateControls()
{
  if (m_rdoCompany->isChecked())
  {
    m_edCpfCnpj->setInputMask("99.999.999/9999-99;_");
    m_edRgIE->setInputMask("");
    m_cbEmployee->setChecked(false);
    m_cbEmployee->setEnabled(false);
  }
  else
  {
    m_edCpfCnpj->setInputMask("999.999.999-99;_");
    m_edRgIE->setInputMask("9999999999;_");
    m_cbEmployee->setEnabled(true);
  }

  emit changedSignal();
}
