#include "personpageview.h"
#include "jlineedit.h"
#include "jpicker.h"
#include "QDateEdit"
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>

PersonPageView::PersonPageView(QWidget* parent)
 : QFrame(parent)
 , m_rdoPerson(nullptr)
 , m_rdoCompany(nullptr)
 , m_edName(nullptr)
 , m_edAlias(nullptr)
 , m_edEmail(nullptr)
 , m_edCpfCnpj(nullptr)
 , m_edRgInsc(nullptr)
 , m_edDetails(nullptr)
 , m_dtBirthDate(nullptr)
 , m_imagePicker(nullptr)
 , m_cbClient(nullptr)
 , m_cbSupplier(nullptr)
 , m_cbEmployee(nullptr)
 , m_frPinCode(nullptr)
{
  m_rdoPerson = new QRadioButton();
  m_rdoPerson->setText(tr("Fisíca"));
  m_rdoPerson->setIcon(QIcon(":/icons/res/house.png"));
  m_rdoPerson->setChecked(true);
  m_rdoCompany = new QRadioButton();
  m_rdoCompany->setText(tr("Jurídica"));
  m_rdoCompany->setIcon(QIcon(":/icons/res/building.png"));
  m_edName = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edAlias = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_edEmail = new JLineEdit(JValidatorType::Email, false, true);
  m_edCpfCnpj = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edRgInsc= new JLineEdit(JValidatorType::Numeric, false, true);
  m_edDetails = new JLineEdit(JValidatorType::AlphanumericAndSpaces, true, true);
  m_dtBirthDate = new QDateEdit();
  m_dtBirthDate->setCalendarPopup(true);
  m_dtBirthDate->setDisplayFormat("dd/MM/yyyy");
  m_dtBirthDate->setDate(QDate::currentDate());
  m_imagePicker = new JPicker(INVALID_IMAGE_ID, tr("Imagem"), true);
  m_cbClient = new QCheckBox();
  m_cbClient->setText(tr("Cliente"));
  m_cbClient->setIcon(QIcon(":/icons/res/client.png"));
  m_cbSupplier = new QCheckBox();
  m_cbSupplier->setText(tr("Fornecedor"));
  m_cbSupplier->setIcon(QIcon(":/icons/res/supplier.png"));
  m_cbEmployee = new QCheckBox();
  m_cbEmployee->setText(tr("Funcionário"));
  m_cbEmployee->setIcon(QIcon(":/icons/res/employee.png"));
  m_frPinCode = new QFrame();
  QLabel* lblPinCode = new QLabel();
  lblPinCode->setPixmap(QIcon(":/icons/res/pincode.png").pixmap(QSize(24, 24)));
  lblPinCode->setMinimumSize(24, 24);
  lblPinCode->setMaximumSize(24, 24);
  lblPinCode->setScaledContents(true);
  m_edPinCode = new JLineEdit(JValidatorType::Numeric, false, true);
  m_edPinCode->setMaxLength(EMPLOYEE_PINCODE_LENGTH);
  m_edPinCode->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);
  m_edPinCode->setAlignment(Qt::AlignCenter);
  {
    QFont font = m_edPinCode->font();
    font.setBold(true);
    m_edPinCode->setFont(font);
  }

  QGroupBox* personGroupBox = new QGroupBox();
  personGroupBox->setTitle(tr("Tipo de pessoa"));
  QHBoxLayout* personLayout = new QHBoxLayout();
  personLayout->addWidget(m_rdoPerson);
  personLayout->addWidget(m_rdoCompany);
  personGroupBox->setLayout(personLayout);

  QFormLayout* flayout0 = new QFormLayout();
  flayout0->setContentsMargins(0, 0, 0, 0);
  flayout0->addRow(tr("Nome/razão social:"), m_edName);
  flayout0->addRow(tr("Apelido/nome fantasia:"), m_edAlias);
  flayout0->addRow(tr("Email:"), m_edEmail);
  flayout0->addRow(tr("CPF/CNPJ:"), m_edCpfCnpj);
  flayout0->addRow(tr("RG/INSC:"), m_edRgInsc);
  flayout0->addRow(tr("Detalhes:"), m_edDetails);

  QGroupBox* dateGroupBox = new QGroupBox();
  dateGroupBox->setCheckable(true);
  dateGroupBox->setTitle(tr("Data de aniversário"));
  QVBoxLayout* dateLayout = new QVBoxLayout();
  dateLayout->addWidget(m_dtBirthDate);
  dateGroupBox->setLayout(dateLayout);

  QHBoxLayout* pinCodeLayout = new QHBoxLayout();
  pinCodeLayout->setContentsMargins(0, 0, 0, 0);
  pinCodeLayout->addWidget(lblPinCode);
  pinCodeLayout->addWidget(m_edPinCode);
  m_frPinCode->setLayout(pinCodeLayout);

  QGroupBox* typeGroupBox = new QGroupBox();
  typeGroupBox->setTitle(tr("Disponível como"));
  QVBoxLayout* typeLayout = new QVBoxLayout();
  typeLayout->addWidget(m_cbClient);
  typeLayout->addWidget(m_cbSupplier);
  typeLayout->addWidget(m_cbEmployee);
  typeLayout->addWidget(m_frPinCode);
  typeGroupBox->setLayout(typeLayout);

  QVBoxLayout* vlayout0 = new QVBoxLayout();
  vlayout0->setAlignment(Qt::AlignTop);
  vlayout0->addWidget(personGroupBox);
  vlayout0->addLayout(flayout0);
  vlayout0->addWidget(dateGroupBox);
  vlayout0->addWidget(m_imagePicker);
  vlayout0->addWidget(typeGroupBox);
  setLayout(vlayout0);
}
