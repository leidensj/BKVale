#include "jtablewidgetitem.h"
#include "jdatabase.h"
#include "tinyexpr.h"
#include "product.h"
#include "productbarcode.h"
#include "packageeditor.h"
#include "timeintervaldlg.h"
#include "jspinbox.h"
#include "jlineedit.h"
#include <QComboBox>
#include <QObject>
#include <QPixmap>
#include <QLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QByteArray>
#include <QDomDocument>
#include <QFormLayout>
#include <QMessageBox>

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

AddressEditorDlg::AddressEditorDlg(QWidget* parent)
  : QDialog(parent)
  , m_edPostalCode(nullptr)
  , m_btnPostalCode(nullptr)
  , m_edNeighborhood(nullptr)
  , m_edStreet(nullptr)
  , m_spnNumber(nullptr)
  , m_edCity(nullptr)
  , m_cbState(nullptr)
  , m_edComplement(nullptr)
  , m_edReference(nullptr)
  , m_btn(nullptr)
{
  setWindowTitle(tr("Endereço"));
  setWindowIcon(QIcon(":/icons/res/address.png"));

  m_edPostalCode = new JLineEdit(JLineEdit::Input::Numeric,
                                 JLineEdit::st_defaultFlags2);
  m_edPostalCode->setInputMask(ADDRESS_CEP_MASK);
  m_btnPostalCode = new QPushButton();
  m_btnPostalCode->setFlat(true);
  m_btnPostalCode->setIconSize(QSize(16, 16));
  m_btnPostalCode->setIcon(QIcon(":/icons/res/process.png"));
  m_btnPostalCode->setToolTip(tr("Buscar CEP"));
  m_edNeighborhood = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                   JLineEdit::st_defaultFlags1);
  m_edNeighborhood->setPlaceholderText(tr("*"));
  m_edStreet = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                             JLineEdit::st_defaultFlags1);
  m_edStreet->setPlaceholderText(tr("*"));
  m_spnNumber = new JSpinBox;
  m_spnNumber->setMinimum(0);
  m_spnNumber->setMaximum(999999);
  m_spnNumber->setPrefix(tr("Nº "));
  m_edCity = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                  JLineEdit::st_defaultFlags1);
  m_edCity->setPlaceholderText(tr("*"));
  m_cbState = new QComboBox();
  for (int i = 0; i != ADDRESS_NUMBER_OF_BRAZILIAN_STATES; ++i)
    m_cbState->addItem(Address::st_getBRState((Address::EBRState)i).m_name);
  m_cbState->setCurrentIndex((int)Address::EBRState::RS);
  m_edComplement = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                 JLineEdit::st_defaultFlags1);
  m_edReference = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                                JLineEdit::st_defaultFlags1);

  QHBoxLayout* ltPostalCode = new QHBoxLayout();
  ltPostalCode->setContentsMargins(0, 0, 0, 0);
  ltPostalCode->addWidget(m_edPostalCode);
  ltPostalCode->addWidget(m_btnPostalCode);

  QHBoxLayout* ltStreet = new QHBoxLayout();
  ltStreet->setContentsMargins(0, 0, 0, 0);
  ltStreet->addWidget(m_edStreet);
  ltStreet->addWidget(m_spnNumber);

  QFormLayout* ltAddress = new QFormLayout();
  ltAddress->setContentsMargins(0, 0, 0, 0);
  ltAddress->addRow(tr("CEP:"), ltPostalCode);
  ltAddress->addRow(tr("Rua:"), ltStreet);
  ltAddress->addRow(tr("Bairro:"), m_edNeighborhood);
  ltAddress->addRow(tr("Cidade:"), m_edCity);
  ltAddress->addRow(tr("Estado:"), m_cbState);
  ltAddress->addRow(tr("Complemento:"), m_edComplement);
  ltAddress->addRow(tr("Referência:"), m_edReference);

  m_btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(ltAddress);
  ltMain->addWidget(m_btn);
  setLayout(ltMain);

  connect(m_btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_btn, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_edStreet, SIGNAL(textChanged(const QString&)), this, SLOT(updateControls()));
  connect(m_edCity, SIGNAL(textChanged(const QString&)), this, SLOT(updateControls()));
  connect(m_edNeighborhood, SIGNAL(textChanged(const QString&)), this, SLOT(updateControls()));
  connect(m_btnPostalCode, SIGNAL(clicked(bool)), this, SLOT(processPostalCode()));

  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  updateControls();
}

void AddressEditorDlg::processPostalCode()
{
  QDomDocument doc;
  bool bSuccess = doc.setContent(searchPostalCode(m_edPostalCode->text()));
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
          m_edNeighborhood->setText(text);
        else if (strNode == "cidade")
          m_edCity->setText(text);
        else if (strNode == "logradouro")
          m_edStreet->setText(text);
        else if (strNode == "estado")
          m_cbState->setCurrentIndex((int)Address::st_getEBRState(text));
      }
    }
  }

  if (!bSuccess)
  {
    QMessageBox::information(this,
                             tr("CEP não encontrado"),
                             tr("Verifique se o CEP '%1' informado "
                                "está correto e se há conexão com a "
                                "Internet.").arg(m_edPostalCode->text()),
                             QMessageBox::Ok);
  }
}

void AddressEditorDlg::updateControls()
{
  auto pt = m_btn->button(QDialogButtonBox::Ok);
  if (pt != nullptr)
    pt->setEnabled(!m_edStreet->text().isEmpty() &&
                   !m_edCity->text().isEmpty() &&
                   !m_edNeighborhood->text().isEmpty());
}

Address AddressEditorDlg::getAddress() const
{
  Address o;
  o.m_id = m_currentId;
  o.m_cep = m_edPostalCode->text();
  o.m_neighborhood = m_edNeighborhood->text();
  o.m_street = m_edStreet->text();
  o.m_number = m_spnNumber->value();
  o.m_city = m_edCity->text();
  o.m_state = (Address::EBRState)m_cbState->currentIndex();
  o.m_complement = m_edComplement->text();
  o.m_reference = m_edReference->text();
  return o;
}

void AddressEditorDlg::setAddress(const Address& o)
{
  m_currentId = o.m_id;
  m_edPostalCode->setText(o.m_cep);
  m_edNeighborhood->setText(o.m_neighborhood);
  m_edStreet->setText(o.m_street);
  m_spnNumber->setValue(o.m_number);
  m_edCity->setText(o.m_city);
  m_cbState->setCurrentIndex((int)o.m_state);
  m_edComplement->setText(o.m_complement);
  m_edReference->setText(o.m_reference);
}

PhoneEditorDlg::PhoneEditorDlg(QWidget* parent)
  : QDialog(parent)
  , m_spnCountryCode(nullptr)
  , m_spnCode(nullptr)
  , m_edNumber(nullptr)
  , m_edName(nullptr)
{
  setWindowTitle(tr("Telefone"));
  setWindowIcon(QIcon(":/icons/res/phone.png"));

  m_edNumber = new JLineEdit(JLineEdit::Input::Numeric,
                             JLineEdit::st_defaultFlags2);
  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                           JLineEdit::st_defaultFlags1);
  m_edName->setPlaceholderText(tr("Nome (opcional)"));
  m_edNumber->setPlaceholderText(tr("*"));
  m_spnCountryCode = new JSpinBox;
  m_spnCountryCode->setMinimum(0);
  m_spnCountryCode->setMaximum(999999);
  m_spnCountryCode->setPrefix("+");
  m_spnCountryCode->setValue(55);
  m_spnCode = new JSpinBox;
  m_spnCode->setMinimum(0);
  m_spnCode->setMaximum(999999);
  m_spnCode->setPrefix("(");
  m_spnCode->setValue(54);
  m_spnCode->setSuffix(")");

  m_btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  QHBoxLayout* ltNumber = new QHBoxLayout;
  ltNumber->setContentsMargins(0, 0, 0, 0);
  ltNumber->addWidget(m_spnCountryCode);
  ltNumber->addWidget(m_spnCode);
  ltNumber->addWidget(m_edNumber);

  QVBoxLayout* ltPhone = new QVBoxLayout;
  ltPhone->addWidget(m_edName);
  ltPhone->addLayout(ltNumber);
  ltPhone->addWidget(m_btn);

  setLayout(ltPhone);

  connect(m_btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_btn, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_edNumber, SIGNAL(textChanged(const QString&)), this, SLOT(updateControls()));

  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  updateControls();
}

void PhoneEditorDlg::updateControls()
{
  auto pt = m_btn->button(QDialogButtonBox::Ok);
  if (pt != nullptr)
    pt->setEnabled(!m_edNumber->text().isEmpty());
}

Phone PhoneEditorDlg::getPhone() const
{
  Phone o;
  o.m_id = m_currentId;
  o.m_countryCode = m_spnCountryCode->value();
  o.m_code = m_spnCode->value();
  o.m_name = m_edName->text();
  o.m_number = m_edNumber->text();
  return o;
}

void PhoneEditorDlg::setPhone(const Phone& o)
{
  m_currentId = o.m_id;
  m_spnCountryCode->setValue(o.m_countryCode);
  m_spnCode->setValue(o.m_code);
  m_edName->setText(o.m_name);
  m_edNumber->setText(o.m_number);
}

DoubleTableWidgetItem::DoubleTableWidgetItem(JItem::DataType type, Color color, bool bCheckable)
  : m_type(type)
  , m_color(color)
  , m_bCheckable(bCheckable)
{
  if (m_bCheckable)
    setFlags(Qt::NoItemFlags |
             Qt::ItemIsSelectable |
             Qt::ItemIsEnabled |
             Qt::ItemIsEditable |
             Qt::ItemIsUserCheckable);
}

void DoubleTableWidgetItem::setValue(double val)
{
  setData(Qt::UserRole, val);
  setText(JItem::st_str(val, m_type));

  if (m_bCheckable)
  {
    if (checkState() == Qt::Unchecked)
      setFlags(flags() & ~Qt::ItemIsEditable);
    else
      setFlags(flags() | Qt::ItemIsEditable);
  }

  switch (m_color)
  {
    case Color::Background:
      setBackgroundColor(val == 0.0 ? QColor(255, 200, 200) : QColor(Qt::white));
      break;
    case Color::Foreground:
      setTextColor(QColor(val >= 0 ? Qt::red : Qt::darkGreen));
      break;
    case Color::None:
    default:
      break;
  }
}

double DoubleTableWidgetItem::getValue()
{
  return data(Qt::UserRole).toDouble();
}

bool DoubleTableWidgetItem::evaluate(const QString& exp)
{
  auto stdExp = exp.toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  if (!error)
    setValue(val);
  return !error;
}

void DoubleTableWidgetItem::evaluate()
{
  if (!evaluate(text()))
    setValue(getValue());
}

PackageTableWidgetItem::PackageTableWidgetItem()
  : QTableWidgetItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags |
           Qt::ItemIsSelectable |
           Qt::ItemIsEnabled);
}

JItemTableWidgetItem::JItemTableWidgetItem()
  : QTableWidgetItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags |
           Qt::ItemIsSelectable |
           Qt::ItemIsEnabled);
}

void ProductTableWidgetItem::setItem(const JItem& o)
{
  m_product = dynamic_cast<const Product&>(o);
  setText(m_product.m_name);

  QPixmap pixmap(QSize(16, 16));
  pixmap.loadFromData(m_product.m_image.m_image);
  setIcon(QIcon(pixmap));
}

const JItem& ProductTableWidgetItem::getItem() const
{
  return m_product;
}

void ProductTableWidgetItem::selectItem(const QString& fixedFilter)
{
  JDatabaseSelector dlg(PRODUCT_SQL_TABLE_NAME,
                        QObject::tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.getDatabase()->setFixedFilter(fixedFilter);
  if (dlg.exec())
  {
    Product* p = static_cast<Product*>(dlg.getDatabase()->getCurrentItem());
    if (p != nullptr && p->m_id.isValid())
      setItem(*p);
  }
}

void ProductTableWidgetItem::selectItemByBarcode(const QString& fixedFilter)
{
  JDatabaseSelector dlg(PRODUCT_BARCODE_SQL_TABLE_NAME,
                        QObject::tr("Selecionar Produto"),
                        QIcon(":/icons/res/item.png"));
  dlg.getDatabase()->setFixedFilter(fixedFilter);
  if (dlg.exec())
  {
    ProductBarcode* p = static_cast<ProductBarcode*>(dlg.getDatabase()->getCurrentItem());
    if (p != nullptr && p->m_id.isValid())
      setItem(p->m_product);
  }
}

void PackageTableWidgetItem::setItem(const Package& o, const QString& productUnity)
{
  m_package = o;
  setText(m_package.strFormattedUnity(productUnity));
}

const Package& PackageTableWidgetItem::getItem() const
{
  return m_package;
}

void PackageTableWidgetItem::selectItem(const QString& productUnity)
{
  PackageEditor dlg(getItem(), productUnity);
  if (dlg.exec())
    setItem(dlg.getPackage(), productUnity);
}

void PersonTableWidgetItem::setItem(const JItem& o)
{
  m_person = dynamic_cast<const Person&>(o);
  setText(m_person.strAliasName());

  QPixmap pixmap(QSize(16, 16));
  pixmap.loadFromData(m_person.m_image.m_image);
  setIcon(QIcon(pixmap));
}

const JItem& PersonTableWidgetItem::getItem() const
{
  return m_person;
}

void PersonTableWidgetItem::selectItem(const QString& fixedFilter)
{
  JDatabaseSelector dlg(PERSON_SQL_TABLE_NAME,
                        QObject::tr("Selecionar Pessoa"),
                        QIcon(":/icons/res/person.png"));
  dlg.getDatabase()->setFixedFilter(fixedFilter);
  if (dlg.exec())
  {
    Person* p = static_cast<Person*>(dlg.getDatabase()->getCurrentItem());
    if (p != nullptr && p->m_id.isValid())
      setItem(*p);
  }
}

TimeIntervalsTableWidgetItem::TimeIntervalsTableWidgetItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags |
           Qt::ItemIsSelectable |
           Qt::ItemIsEnabled);
}

void TimeIntervalsTableWidgetItem::selectItem()
{
  TimeIntervalDlg dlg;
  dlg.setItems(m_timeIntervals);
  if (dlg.exec())
    setItems(dlg.getItems());
}

void TimeIntervalsTableWidgetItem::setItems(const QVector<TimeInterval>& v)
{
  m_timeIntervals = v;
  QString str;
  for (int i = 0; i != v.size(); ++i)
    str += v.at(i).m_tmBegin.toString("hh:mm") + "-" + v.at(i).m_tmEnd.toString("hh:mm") + " ";
  if (v.size() > 0)
    str.chop(1);
  setText(str);
}

const QVector<TimeInterval>& TimeIntervalsTableWidgetItem::getItems() const
{
  return m_timeIntervals;
}

PhoneEditorTableWidgetItem::PhoneEditorTableWidgetItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags |
           Qt::ItemIsSelectable |
           Qt::ItemIsEnabled);
}

void PhoneEditorTableWidgetItem::setItem(const Phone& o)
{
  m_o = o;
  setText(o.strFormattedPhoneWithName());
}

const Phone& PhoneEditorTableWidgetItem::getItem() const
{
  return m_o;
}

bool PhoneEditorTableWidgetItem::selectItem()
{
  PhoneEditorDlg dlg;
  dlg.setPhone(m_o);
  if (dlg.exec())
  {
    setItem(dlg.getPhone());
    return true;
  }
  return false;
}

AddressEditorTableWidgetItem::AddressEditorTableWidgetItem()
{
  setTextColor(QColor(Qt::darkGray));
  setFlags(Qt::NoItemFlags |
           Qt::ItemIsSelectable |
           Qt::ItemIsEnabled);
}

void AddressEditorTableWidgetItem::setItem(const Address& o)
{
  m_o = o;
  setText(o.getFormattedAddress());
}

const Address& AddressEditorTableWidgetItem::getItem() const
{
  return m_o;
}

bool AddressEditorTableWidgetItem::selectItem()
{
  AddressEditorDlg dlg;
  dlg.setAddress(m_o);
  if (dlg.exec())
  {
    setItem(dlg.getAddress());
    return true;
  }
  return false;
}
