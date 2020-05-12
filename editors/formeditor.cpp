#include "formeditor.h"
#include "widgets/jlineedit.h"
#include "controls/databasepicker.h"
#include "widgets/jaddremovebuttons.h"
#include "widgets/jplaintextedit.h"
#include "tables/addresstable.h"
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QDateEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QMessageBox>
#include <QAction>
#include <QGroupBox>
#include <QPushButton>

FormInfoEditor::FormInfoEditor(QWidget* parent)
  : QWidget(parent)
  , m_dtCreationDate(nullptr)
  , m_type(nullptr)
  , m_rdoPerson(nullptr)
  , m_rdoCompany(nullptr)
  , m_edName(nullptr)
  , m_edAlias(nullptr)
  , m_lblType(nullptr)
  , m_lblCreationDate(nullptr)
  , m_lblName(nullptr)
  , m_lblAlias(nullptr)
{
  m_dtCreationDate = new QDateEdit;
  m_dtCreationDate->setCalendarPopup(true);
  m_dtCreationDate->setDisplayFormat("dd/MM/yyyy");
  m_dtCreationDate->setDate(QDate::currentDate());
  m_dtCreationDate->setReadOnly(true);
  m_rdoPerson = new QRadioButton;
  m_rdoPerson->setText(tr("Fisíca"));
  m_rdoPerson->setChecked(true);
  m_rdoCompany = new QRadioButton;
  m_rdoCompany->setText(tr("Jurídica"));
  m_edName = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edAlias = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_lblName = new QLabel;
  m_lblAlias = new QLabel;
  m_lblCreationDate = new QLabel(tr("Data de criação:"));
  m_lblType = new QLabel(tr("Tipo:"));

  QHBoxLayout* ltType = new QHBoxLayout;
  ltType->setContentsMargins(0, 0, 0, 0);
  ltType->addWidget(m_rdoPerson);
  ltType->addWidget(m_rdoCompany);
  ltType->setAlignment(Qt::AlignLeft);

  m_type = new QWidget;
  m_type->setLayout(ltType);

  QFormLayout* lt = new QFormLayout;
  lt->addRow(m_lblCreationDate, m_dtCreationDate);
  lt->addRow(m_lblType, m_type);
  lt->addRow(m_lblName, m_edName);
  lt->addRow(m_lblAlias, m_edAlias);

  connect(m_rdoCompany, SIGNAL(clicked(bool)), this, SLOT(switchUserType()));
  connect(m_rdoPerson, SIGNAL(clicked(bool)), this, SLOT(switchUserType()));
  switchUserType();
  m_edName->setFocus();
  setLayout(lt);
}

void FormInfoEditor::addWidget(const QString& text, QWidget* w)
{
  if (w != nullptr)
  {
    auto l = dynamic_cast<QFormLayout*>(layout());
    l->addRow(text, w);
  }
}

void FormInfoEditor::setCompany(bool b)
{
  m_rdoCompany->setChecked(b);
  m_rdoPerson->setChecked(!b);
  switchUserType();
}

void FormInfoEditor::setForm(const Form& o)
{
  m_edName->setText(o.m_name);
  m_edAlias->setText(o.m_alias);
  m_dtCreationDate->setDate(o.m_dtCreation);
  m_rdoCompany->setChecked(o.m_bCompany);
  m_rdoPerson->setChecked(!o.m_bCompany);
  switchUserType();
}

void FormInfoEditor::fillForm(Form& o) const
{
  o.m_name = m_edName->text();
  o.m_alias = m_edAlias->text();
  o.m_dtCreation = m_dtCreationDate->date();
  o.m_bCompany = m_rdoCompany->isChecked();
}

void FormInfoEditor::switchUserType()
{
  if (m_rdoCompany->isChecked())
  {
    m_lblName->setText(tr("Razão social:"));
    m_lblAlias->setText(tr("Nome fantasia:"));
  }
  else
  {
    m_lblName->setText(tr("Nome:"));
    m_lblAlias->setText(tr("Apelido:"));
  }

  emit userTypeChangedSignal(m_rdoCompany->isChecked());
}

FormDetailsEditor::FormDetailsEditor(QWidget* parent)
  : QWidget(parent)
  , m_bPreviousWasCompany(false)
  , m_edCpfCnpj(nullptr)
  , m_lblCpfCnpj(nullptr)
  , m_edRgIE(nullptr)
  , m_lblRgIE(nullptr)
  , m_edEmail(nullptr)
  , m_teDetails(nullptr)
  , m_dtBirthDate(nullptr)
  , m_cbBirthDate(nullptr)
  , m_imagePicker(nullptr)
{
  m_edCpfCnpj = new JLineEdit(Text::Input::Numeric, false);
  m_edRgIE= new JLineEdit(Text::Input::Numeric, false);
  m_lblCpfCnpj = new QLabel;
  m_lblRgIE = new QLabel;
  m_edEmail = new JLineEdit(Text::Input::All, false);
  m_teDetails = new JPlainTextEdit;
  m_dtBirthDate = new QDateEdit;
  m_dtBirthDate->setCalendarPopup(true);
  m_dtBirthDate->setDisplayFormat("dd/MM/yyyy");
  m_dtBirthDate->setDate(QDate::currentDate());
  m_cbBirthDate = new QCheckBox;
  m_cbBirthDate->setChecked(false);
  m_imagePicker = new DatabasePicker(IMAGE_SQL_TABLE_NAME);

  QFormLayout* lt = new QFormLayout;
  lt->addRow(m_lblCpfCnpj, m_edCpfCnpj);
  lt->addRow(m_lblRgIE, m_edRgIE);
  lt->addRow(tr("Email:"), m_edEmail);
  lt->addRow(tr("Imagem:"), m_imagePicker);
  lt->addRow(m_cbBirthDate, m_dtBirthDate);
  lt->addRow(tr("Detalhes:"), m_teDetails);

  connect(m_cbBirthDate, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  setLayout(lt);
  updateControls();
}

void FormDetailsEditor::setForm(const Form& o)
{
  m_bPreviousWasCompany = o.m_bCompany;
  m_strPreviousCpfCnpj = o.m_CPF_CNPJ;
  m_strPreviousRgIE = o.m_RG_IE;

  m_imagePicker->addItem(o.m_image);
  m_edEmail->setText(o.m_email);
  m_edCpfCnpj->setText(o.m_CPF_CNPJ);
  m_edRgIE->setText(o.m_RG_IE);
  m_teDetails->setPlainText(o.m_details);
  m_cbBirthDate->setChecked(o.m_bBirth);
  m_dtBirthDate->setDate(o.m_dtBirth);
  switchUserType(o.m_bCompany);
}

void FormDetailsEditor::fillForm(Form& o) const
{
  o.m_image.m_id = m_imagePicker->getFirstId();
  o.m_email = m_edEmail->text();
  o.m_CPF_CNPJ = m_edCpfCnpj->text();
  o.m_RG_IE = m_edRgIE->text();
  o.m_details = m_teDetails->toPlainText();
  o.m_bBirth = m_cbBirthDate->isChecked();
  o.m_dtBirth = m_dtBirthDate->date();
}

void FormDetailsEditor::switchUserType(bool bCompany)
{
  if (bCompany)
  {
    m_lblCpfCnpj->setText(tr("CNPJ:"));
    m_lblRgIE->setText(tr("IE:"));
    m_cbBirthDate->setText("Fundação:");
    m_edCpfCnpj->setInputMask("99.999.999/9999-99;_");
    m_edCpfCnpj->setText(m_bPreviousWasCompany ? m_strPreviousCpfCnpj : "");
    m_edRgIE->setInputMask("");
    m_edRgIE->setText(m_bPreviousWasCompany ? m_strPreviousRgIE : "");
  }
  else
  {
    m_lblCpfCnpj->setText(tr("CPF:"));
    m_lblRgIE->setText(tr("RG:"));
    m_cbBirthDate->setText("Aniversário:");
    m_edCpfCnpj->setInputMask("999.999.999-99;_");
    m_edCpfCnpj->setText(!m_bPreviousWasCompany ? m_strPreviousCpfCnpj : "");
    m_edRgIE->setInputMask("9999999999;_");
    m_edRgIE->setText(!m_bPreviousWasCompany ? m_strPreviousRgIE : "");
  }
}

void FormDetailsEditor::updateControls()
{
  m_dtBirthDate->setEnabled(m_cbBirthDate->isChecked());
  if (m_cbBirthDate->isChecked() &&
      m_dtBirthDate->date() == m_dtBirthDate->minimumDate())
    m_dtBirthDate->setDate(QDate::currentDate());
}

FormPhoneEditor::FormPhoneEditor(QWidget* parent)
  : QWidget(parent)
  , m_btnAddRemove(nullptr)
  , m_tbPhone(nullptr)
{
  m_btnAddRemove = new JAddRemoveButtons;
  m_tbPhone = new PhoneTable(m_btnAddRemove);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_btnAddRemove);
  lt->addWidget(m_tbPhone);
  setLayout(lt);
}

void FormPhoneEditor::setForm(const Form& o)
{
  m_tbPhone->setPhones(o.m_vPhone);
}

void FormPhoneEditor::fillForm(Form& o) const
{
  QVector<Phone> v;
  m_tbPhone->getPhones(v);
  o.m_vPhone.append(v);
}

FormAddressEditor::FormAddressEditor(QWidget* parent)
  : QWidget(parent)
  , m_btnAddRemove(nullptr)
  , m_tbAddress(nullptr)
{
  m_btnAddRemove = new JAddRemoveButtons;
  m_tbAddress = new AddressTable(m_btnAddRemove);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_btnAddRemove);
  lt->addWidget(m_tbAddress);

  setLayout(lt);
}

void FormAddressEditor::setForm(const Form& o)
{
  m_tbAddress->setAddresses(o.m_vAddress);
}

void FormAddressEditor::fillForm(Form& o) const
{
  m_tbAddress->getAddresses(o.m_vAddress);
}
