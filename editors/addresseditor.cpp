#include "addresseditor.h"
#include "widgets/jspinbox.h"
#include "widgets/jlineedit.h"
#include <QComboBox>
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

AddressEditor::AddressEditor(QWidget* parent)
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

  m_edPostalCode = new JLineEdit(Text::Input::Numeric, false);
  m_edPostalCode->setInputMask(ADDRESS_CEP_MASK);
  m_btnPostalCode = new QPushButton();
  m_btnPostalCode->setFlat(true);
  m_btnPostalCode->setIconSize(QSize(16, 16));
  m_btnPostalCode->setIcon(QIcon(":/icons/res/process.png"));
  m_btnPostalCode->setToolTip(tr("Buscar CEP"));
  m_edNeighborhood = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edNeighborhood->setPlaceholderText(tr("*"));
  m_edStreet = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edStreet->setPlaceholderText(tr("*"));
  m_spnNumber = new JSpinBox;
  m_spnNumber->setMinimum(0);
  m_spnNumber->setMaximum(999999);
  m_spnNumber->setPrefix(tr("Nº "));
  m_edCity = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edCity->setPlaceholderText(tr("*"));
  m_cbState = new QComboBox();
  for (int i = 0; i != ADDRESS_NUMBER_OF_BRAZILIAN_STATES; ++i)
    m_cbState->addItem(Address::st_getBRState((Address::EBRState)i).m_name);
  m_cbState->setCurrentIndex((int)Address::EBRState::RS);
  m_edComplement = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);
  m_edReference = new JLineEdit(Text::Input::AlphanumericAndSpaces, true);

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

void AddressEditor::processPostalCode()
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

void AddressEditor::updateControls()
{
  auto pt = m_btn->button(QDialogButtonBox::Ok);
  if (pt != nullptr)
    pt->setEnabled(!m_edStreet->text().isEmpty() &&
                   !m_edCity->text().isEmpty() &&
                   !m_edNeighborhood->text().isEmpty());
}

Address AddressEditor::getAddress() const
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

void AddressEditor::setAddress(const Address& o)
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
