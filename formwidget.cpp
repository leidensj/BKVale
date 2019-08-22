#include "formwidget.h"
#include "widgets/jlineedit.h"
#include "widgets/jdatabasepicker.h"
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

FormInfoWidget::FormInfoWidget(QWidget* parent)
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
  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
  m_edAlias = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces, JLineEdit::st_defaultFlags1);
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

void FormInfoWidget::addWidget(const QString& text, QWidget* w)
{
  if (w != nullptr)
  {
    auto l = dynamic_cast<QFormLayout*>(layout());
    l->addRow(text, w);
  }
}

void FormInfoWidget::setCompany(bool b)
{
  m_rdoCompany->setChecked(b);
  m_rdoPerson->setChecked(!b);
  switchUserType();
}

void FormInfoWidget::setForm(const Form& o)
{
  m_edName->setText(o.m_name);
  m_edAlias->setText(o.m_alias);
  m_dtCreationDate->setDate(o.m_dtCreation);
  m_rdoCompany->setChecked(o.m_bCompany);
  m_rdoPerson->setChecked(!o.m_bCompany);
  switchUserType();
}

void FormInfoWidget::fillForm(Form& o) const
{
  o.m_name = m_edName->text();
  o.m_alias = m_edAlias->text();
  o.m_dtCreation = m_dtCreationDate->date();
  o.m_bCompany = m_rdoCompany->isChecked();
}

void FormInfoWidget::switchUserType()
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

FormDetailsWidget::FormDetailsWidget(QWidget* parent)
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
  m_edCpfCnpj = new JLineEdit(JLineEdit::Input::Numeric, JLineEdit::st_defaultFlags2);
  m_edRgIE= new JLineEdit(JLineEdit::Input::Numeric, JLineEdit::st_defaultFlags2);
  m_lblCpfCnpj = new QLabel;
  m_lblRgIE = new QLabel;
  m_edEmail = new JLineEdit(JLineEdit::Input::All, JLineEdit::st_defaultFlags2);
  m_teDetails = new JPlainTextEdit;
  m_dtBirthDate = new QDateEdit;
  m_dtBirthDate->setCalendarPopup(true);
  m_dtBirthDate->setDisplayFormat("dd/MM/yyyy");
  m_dtBirthDate->setDate(QDate::currentDate());
  m_cbBirthDate = new QCheckBox;
  m_cbBirthDate->setChecked(false);
  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME);

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

void FormDetailsWidget::setForm(const Form& o)
{
  m_bPreviousWasCompany = o.m_bCompany;
  m_strPreviousCpfCnpj = o.m_CPF_CNPJ;
  m_strPreviousRgIE = o.m_RG_IE;

  m_imagePicker->setItem(o.m_image);
  m_edEmail->setText(o.m_email);
  m_edCpfCnpj->setText(o.m_CPF_CNPJ);
  m_edRgIE->setText(o.m_RG_IE);
  m_teDetails->setPlainText(o.m_details);
  m_cbBirthDate->setChecked(o.m_bBirth);
  m_dtBirthDate->setDate(o.m_dtBirth);
  switchUserType(o.m_bCompany);
}

void FormDetailsWidget::fillForm(Form& o) const
{
  o.m_image.m_id = m_imagePicker->getId();
  o.m_email = m_edEmail->text();
  o.m_CPF_CNPJ = m_edCpfCnpj->text();
  o.m_RG_IE = m_edRgIE->text();
  o.m_details = m_teDetails->toPlainText();
  o.m_bBirth = m_cbBirthDate->isChecked();
  o.m_dtBirth = m_dtBirthDate->date();
}

void FormDetailsWidget::switchUserType(bool bCompany)
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

void FormDetailsWidget::updateControls()
{
  m_dtBirthDate->setEnabled(m_cbBirthDate->isChecked());
  if (m_cbBirthDate->isChecked() &&
      m_dtBirthDate->date() == m_dtBirthDate->minimumDate())
    m_dtBirthDate->setDate(QDate::currentDate());
}

FormPhoneWidget::FormPhoneWidget(QWidget* parent)
  : QWidget(parent)
  , m_btnAddRemove(nullptr)
  , m_tbPhone(nullptr)
{
  m_tbPhone = new JTable;
  m_btnAddRemove = new JAddRemoveButtons;

  m_tbPhone->setColumnCount(4);
  QStringList headers;
  headers << "País" << "Código" << "Número" << "Nome";
  m_tbPhone->setHorizontalHeaderLabels(headers);
  m_tbPhone->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
  m_tbPhone->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
  m_tbPhone->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
  m_tbPhone->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::Stretch);

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_btnAddRemove);
  lt->addWidget(m_tbPhone);

  connect(m_btnAddRemove->m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addPhone()));
  connect(m_btnAddRemove->m_btnRemove, SIGNAL(clicked(bool)), m_tbPhone, SLOT(removeItem()));
  connect(m_tbPhone, SIGNAL(changedSignal(bool)), this, SLOT(updateControls()));
  connect(m_tbPhone, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateTable(QTableWidgetItem*)));
  setLayout(lt);
}

void FormPhoneWidget::setForm(const Form& o)
{
  setPhones(o.m_vPhone);
}

void FormPhoneWidget::addPhone()
{
  m_tbPhone->insertRow(m_tbPhone->rowCount());
  int row = m_tbPhone->rowCount() - 1;

  auto itCountryCode = new DoubleItem(JItem::DataType::Integer, DoubleItem::Color::None, false, "+");
  auto itCode = new DoubleItem(JItem::DataType::Integer, DoubleItem::Color::None, false, "(", ")");
  auto itNumber = new QTableWidgetItem;
  auto itName = new QTableWidgetItem;


  m_tbPhone->setItem(row, 0, itCountryCode);
  m_tbPhone->setItem(row, 1, itCode);
  m_tbPhone->setItem(row, 2, itNumber);
  m_tbPhone->setItem(row, 3, itName);

  Phone o;
  itCountryCode->setValue(o.m_countryCode);
  itCode->setValue(o.m_code);

  m_tbPhone->setCurrentItem(itCountryCode);
  m_tbPhone->setFocus();
}

void FormPhoneWidget::setPhones(const QVector<Phone>& v)
{
  m_tbPhone->removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addPhone();
    dynamic_cast<DoubleItem*>(m_tbPhone->item(i, 0))->setValue(v.at(i).m_countryCode);
    dynamic_cast<DoubleItem*>(m_tbPhone->item(i, 1))->setValue(v.at(i).m_code);
    m_tbPhone->item(i, 2)->setText(v.at(i).m_number);
    m_tbPhone->item(i, 3)->setText(v.at(i).m_name);
  }
}

void FormPhoneWidget::getPhones(QVector<Phone>& v) const
{
  v.clear();
  for (int i = 0; i != m_tbPhone->rowCount(); ++i)
  {
    Phone o;
    o.m_countryCode += (int)dynamic_cast<DoubleItem*>(m_tbPhone->item(i, 0))->getValue();
    o.m_code = (int)dynamic_cast<DoubleItem*>(m_tbPhone->item(i, 1))->getValue();
    o.m_number = m_tbPhone->item(i, 2)->text();
    o.m_name = m_tbPhone->item(i, 3)->text();
    v.push_back(o);
  }
}

void FormPhoneWidget::fillForm(Form& o) const
{
  QVector<Phone> v;
  getPhones(v);
  o.m_vPhone.append(v);
}

void FormPhoneWidget::updateTable(QTableWidgetItem* p)
{
  switch (p->column())
  {
    case 0:
    case 1:
      dynamic_cast<ExpItem*>(p)->evaluate();
      break;
    default:
      break;
  }
}

void FormPhoneWidget::updateControls()
{
  m_btnAddRemove->m_btnRemove->setEnabled(m_tbPhone->isValidCurrentRow());
}

FormAddressWidget::FormAddressWidget(QWidget* parent)
  : QWidget(parent)
  , m_btnAddRemove(nullptr)
  , m_tbAddress(nullptr)
{
  m_tbAddress = new AddressTable;
  m_btnAddRemove = new JAddRemoveButtons;

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_btnAddRemove);
  lt->addWidget(m_tbAddress);

  connect(m_btnAddRemove->m_btnAdd, SIGNAL(clicked(bool)), m_tbAddress, SLOT(addItem()));
  connect(m_btnAddRemove->m_btnRemove, SIGNAL(clicked(bool)), m_tbAddress, SLOT(removeItem()));
  connect(m_tbAddress, SIGNAL(changedSignal(bool)), this, SLOT(updateControls()));
  setLayout(lt);
}

void FormAddressWidget::setForm(const Form& o)
{
  m_tbAddress->removeAllItems();
  for (int i = 0; i != o.m_vAddress.size(); ++i)
    m_tbAddress->addItem(o.m_vAddress.at(i));
}

void FormAddressWidget::fillForm(Form& o) const
{
  for (int i = 0; i != m_tbAddress->rowCount(); ++i)
    o.m_vAddress.push_back(dynamic_cast<const Address&>(m_tbAddress->getItem(i)));
}

void FormAddressWidget::updateControls()
{
  m_btnAddRemove->m_btnRemove->setEnabled(m_tbAddress->isValidCurrentRow());
}
