#include "purchasereport.h"
#include "items/purchase.h"
#include "controls/databasepicker.h"
#include "widgets/jdateinterval.h"
#include <QLayout>
#include <QFrame>
#include <QRadioButton>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

PurchaseReport::PurchaseReport(QWidget* parent)
 : PurchaseFilter(parent)
 , m_rdoPurchase(nullptr)
 , m_rdoProduct(nullptr)
 , m_rdoSupplier(nullptr)
{
  m_btnClear->setIconSize(QSize(16, 16));
  m_dtInt->layout()->setContentsMargins(0, 9, 0, 0);
  m_dtInt->setFlat(true);
  m_rdoPurchase = new QRadioButton(tr("Compra"));
  m_rdoPurchase->setIcon(QIcon(":/icons/res/purchase.png"));
  m_rdoPurchase->setChecked(true);
  m_rdoProduct = new QRadioButton(tr("Produto"));
  m_rdoProduct->setIcon(QIcon(":/icons/res/item.png"));
  m_rdoSupplier = new QRadioButton(tr("Fornecedor"));
  m_rdoSupplier->setIcon(QIcon(":/icons/res/supplier.png"));

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  m_ltButton->addWidget(vFrame1);
  m_ltButton->addWidget(new QLabel(tr("Agrupar:")));
  m_ltButton->addWidget(m_rdoPurchase);
  m_ltButton->addWidget(m_rdoProduct);
  m_ltButton->addWidget(m_rdoSupplier);
  m_dtInt->week();
}

QString PurchaseReport::getProductFilter() const
{
  QString str;
  QVector<Id> vProduct = m_productPicker->getIds();
  if (!vProduct.isEmpty())
  {
    str += " _NOTE_ITEMS._PRODUCTID IN (";
    for (int i = 0; i != vProduct.size(); ++i)
      str += vProduct.at(i).str() + ",";
    str.chop(1);
    str += ") ";
  }
  return str;
}

QString PurchaseReport::process() const
{
  if (m_rdoProduct->isChecked())
    return processProduct();
  return processPurchase();
}

#include "controls/databaseviewer.h"

QString PurchaseReport::strFilterHtml() const
{
  QString str("<table>");
  QString aux;
  if (!m_dtInt->isChecked())
    aux = tr("não especificado");
  else
    aux = m_dtInt->getInitialDate().toString("dd/MM/yyyy") +
          tr(" a ")
          + m_dtInt->getFinalDate().toString("dd/MM/yyyy");
  str += QString("<tr><td>Data: %1</td></tr>").arg(aux);

  auto vs = m_supplierPicker->getNames();
  aux.clear();
  if (!vs.isEmpty())
  {
    for (int i = 0; i != vs.size(); ++i)
      aux += vs.at(i) + ";";
    aux.chop(1);
  }
  if (aux.isEmpty())
    aux = tr("não especificado");
  str += QString("<tr><td>Fornecedor: %1</td></tr>").arg(aux);

  auto vp = m_productPicker->getNames();
  aux.clear();
  if (!vp.isEmpty())
  {
    for (int i = 0; i != vp.size(); ++i)
      aux += vp.at(i) + ";";
    aux.chop(1);
  }
  if (aux.isEmpty())
    aux = tr("não especificado");
  str += QString("<tr><td>Produto: %1</td></tr>").arg(aux);

  auto vst = m_storePicker->getNames();
  aux.clear();
  if (!vst.isEmpty())
  {
    for (int i = 0; i != vst.size(); ++i)
      aux += vst.at(i) + ";";
    aux.chop(1);
  }
  if (aux.isEmpty())
    aux = tr("não especificado");
  str += QString("<tr><td>Loja: %1</td></tr>").arg(aux);

  aux.clear();
  if (!m_cbPaymentCredit->isChecked() && !m_cbPaymentCash->isChecked() && !m_cbPaymentBonus->isChecked())
    aux = tr("não especificado");
  else
  {
    if (m_cbPaymentCredit->isChecked())
      str += Purchase::st_paymentText(Purchase::PaymentMethod::Credit) + ";";
    if(m_cbPaymentCash->isChecked())
      aux += Purchase::st_paymentText(Purchase::PaymentMethod::Cash)  + ";";
    if (m_cbPaymentBonus->isChecked())
      aux += Purchase::st_paymentText(Purchase::PaymentMethod::Bonus)  + ";";
    str.chop(1);
  }
  str += QString("<tr><td>Pagamento: %1</td></tr>").arg(aux);
  return str += "</table>";
}

QString PurchaseReport::processProduct() const
{
  QString strQuery = "SELECT _NOTES._NUMBER AS NUMBER," //0
                     "_NOTES._DATE AS _DATE," //1
                     "_NOTES._OBSERVATION AS OBSERVATION," //2
                     "_NOTES._DISCCOUNT AS DISCCOUNT," //3
                     "_NOTES._PAYMENT_METHOD AS PAYMENT," //4
                     "SUPPLIERFORM._NAME AS SUPPLIER," //5
                     "STOREFORM._NAME AS STORE," //6
                     "_PRODUCTS._NAME AS PRODUCT," //7
                     "_PRODUCTS._UNITY AS UNITY," //8
                     "_NOTE_ITEMS._PRODUCTID AS PRODUCTID," //9
                     "_NOTE_ITEMS._AMMOUNT AS AMMOUNT," //10
                     "_NOTE_ITEMS._PRICE AS PRICE," //11
                     "_NOTE_ITEMS._IS_PACK AS ISPACK," //12
                     "_NOTE_ITEMS._PACK_UNITY AS PACKUNITY," //13
                     "_NOTE_ITEMS._PACK_AMMOUNT AS PACKAMMOUNT," //14
                     "_NOTE_ITEMS._PRICE * _NOTE_ITEMS._AMMOUNT AS SUBTOTAL " //15
                     "FROM _NOTE_ITEMS "
                     "LEFT JOIN _NOTES ON _NOTE_ITEMS._NOTEID = _NOTES._ID "
                     "FULL JOIN _SUPPLIERS ON _NOTES._SUPPLIERID = _SUPPLIERS._ID "
                     "LEFT JOIN _FORMS AS SUPPLIERFORM ON _SUPPLIERS._FORMID = SUPPLIERFORM._ID "
                     "LEFT JOIN _STORES ON _NOTES._STOREID = _STORES._ID "
                     "LEFT JOIN _FORMS AS STOREFORM ON _STORES._FORMID = STOREFORM._ID "
                     "LEFT JOIN _PRODUCTS ON _NOTE_ITEMS._PRODUCTID = _PRODUCTS._ID";
  if (!getFilter().isEmpty())
    strQuery += " WHERE " + getFilter();
  strQuery += " ORDER BY PRODUCT ASC, _DATE DESC, NUMBER DESC, SUPPLIER ASC";
  QSqlQuery query(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  if (!(query.exec(strQuery) && query.next()))
    return "";

  QString html("<html><body><h1 align=\"center\">Relatório de Compras</h1>" + strFilterHtml());
  qlonglong currentProduct = -1;
  double currentSubtotal = 0.0;
  double currentAmmount = 0.0;
  do
  {
    if (currentProduct != query.value(9).toLongLong())
    {
      currentProduct = query.value(9).toLongLong();
      html += QString("<br><table cellspacing=\"0\" cellpadding=\"3\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: black;\">"
                      "<thead><tr><th align=\"left\" colspan=\"6\">%1</th></tr>").arg(query.value(7).toString());
      html += "<tr><th>Data</th><th>Número</th><th>Fornecedor</th><th>Loja</th><th>Quantidade</th><th>Subtotal</th></tr>";
    }
    html += QString(
                "<tr>"
                "<td width=\"7%\">%1</td>"
                "<td width=\"7%\">%2</td>"
                "<td width=\"28%\">%3</td>"
                "<td width=\"28%\">%4</td>"
                "<td width=\"20%\">%5</td>"
                "<td width=\"10%\">%6</td>"
                "</tr>").arg(query.value(1).toDate().toString("dd/MM/yyyy"),
                             Data::strInt(query.value(0).toLongLong()),
                             query.value(5).toString(),
                             query.value(6).toString(),
                             Data::strAmmount(query.value(10).toDouble()) +
                             (query.value(12).toBool()
                              ? query.value(13).toString()
                              : query.value(8).toString()) +
                             " x " + Data::strMoney(query.value(11).toDouble()),
                             Data::strMoney(query.value(15).toDouble()));
    currentSubtotal += query.value(15).toDouble();
    double ammount = query.value(10).toDouble();
    if (query.value(12).toBool()) // isPack
      ammount *= query.value(14).toDouble();
    currentAmmount += ammount;

    bool bHasNext = query.next();
    bool bPrintFooter = !bHasNext || currentProduct != query.value(9).toLongLong();
    query.previous();

    if (bPrintFooter)
    {
      html += "</table><table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">";
      html += QString(
                  "<tr><th align=\"right\">Sumário: %1</td></tr>"
                  "<tr><td align=\"right\">Quantidade: %2</td></tr>"
                  "<tr><td align=\"right\">Média de preço: %3</td></tr>"
                  "<tr><td align=\"right\">Total: %4</td></tr></table>").arg(query.value(7).toString(),
                                                                             Data::strAmmount(currentAmmount) + query.value(8).toString(),
                                                                             Data::strMoney(currentSubtotal / currentAmmount),
                                                                             Data::strMoney(currentSubtotal));
      currentSubtotal = 0.0;
      currentAmmount = 0.0;
    }
  } while (query.next());

  html += "<hr></body></html>";
  return html;
}

QString PurchaseReport::processPurchase() const
{
  QString strQuery = "SELECT _NOTES._NUMBER AS NUMBER," //0
                     "_NOTES._DATE AS _DATE," //1
                     "_NOTES._OBSERVATION AS OBSERVATION," //2
                     "_NOTES._DISCCOUNT AS DISCCOUNT," //3
                     "_NOTES._PAYMENT_METHOD AS PAYMENT," //4
                     "SUPPLIERFORM._NAME AS SUPPLIER," //5
                     "STOREFORM._NAME AS STORE," //6
                     "_PRODUCTS._NAME AS PRODUCT," //7
                     "_PRODUCTS._UNITY AS UNITY," //8
                     "_NOTE_ITEMS._PRODUCTID AS PRODUCTID," //9
                     "_NOTE_ITEMS._AMMOUNT AS AMMOUNT," //10
                     "_NOTE_ITEMS._PRICE AS PRICE," //11
                     "_NOTE_ITEMS._IS_PACK AS ISPACK," //12
                     "_NOTE_ITEMS._PACK_UNITY AS PACKUNITY," //13
                     "_NOTE_ITEMS._PACK_AMMOUNT AS PACKAMMOUNT," //14
                     "_NOTE_ITEMS._PRICE * _NOTE_ITEMS._AMMOUNT AS SUBTOTAL " //15
                     "FROM _NOTE_ITEMS "
                     "FULL JOIN _NOTES ON _NOTE_ITEMS._NOTEID = _NOTES._ID "
                     "FULL JOIN _SUPPLIERS ON _NOTES._SUPPLIERID = _SUPPLIERS._ID "
                     "LEFT JOIN _FORMS AS SUPPLIERFORM ON _SUPPLIERS._FORMID = SUPPLIERFORM._ID "
                     "LEFT JOIN _STORES ON _NOTES._STOREID = _STORES._ID "
                     "LEFT JOIN _FORMS AS STOREFORM ON _STORES._FORMID = STOREFORM._ID "
                     "LEFT JOIN _PRODUCTS ON _NOTE_ITEMS._PRODUCTID = _PRODUCTS._ID";
  if (!getFilter().isEmpty())
    strQuery += " WHERE " + getFilter();
  strQuery += " ORDER BY NUMBER DESC";
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  QSqlQuery query(db);
  if (!(query.exec(strQuery) && query.next()))
      return "";

  int currentNumber = -1;
  double currentSubtotal = 0.0;
  double currentTotal = 0.0;
  int nPurchases = 0;
  QString html("<html><body><h1 align=\"center\">Relatório de Compras</h1>" + strFilterHtml());

  do
  {
    const bool bHasProduct = query.value(9).toInt() != 0 || query.value(15).toDouble() != 0.0;
    if (currentNumber != query.value(0).toInt())
    {
      html += QString(
      "<br><table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">"
        "<tr><td width=\"30%\">Número: %1</td>"
        "<td width=\"30%\">Data: %2</td>"
        "<td width=\"40%\">Pagamento: %3</td></tr></table>"
      "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">"
        "<tr><td width=\"50%\">Fornecedor: %4</td>"
            "<td width=\"50%\">Loja: %5</td></tr>"
      "</table>").arg(Data::strInt(query.value(0).toInt()),
                      query.value(1).toDate().toString("dd/MM/yyyy dddd"),
                      Purchase::st_paymentText((Purchase::PaymentMethod)query.value(4).toInt()),
                      query.value(5).toString(),
                      query.value(6).toString());
      if (bHasProduct)
      {
        html +=
        "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: black;\">"
          "<tr><th>Quantidade</th><th>Produto</th><th>Subtotal</th></tr>";
      }
      currentNumber = query.value(0).toInt();
      nPurchases++;
    }
    if (bHasProduct)
    {
      html += QString(
      "<tr>"
        "<td width=\"25%\">%1</td>"
        "<td width=\"50%\">%2</td>"
        "<td width=\"25%\">%3</td>"
      "</tr>").arg(Data::strAmmount(query.value(10).toDouble()) +
                    (query.value(12).toBool()
                    ? query.value(13).toString()
                    : query.value(8).toString()) +
                    " x " + Data::strMoney(query.value(11).toDouble()),
                   query.value(7).toString(),
                   Data::strMoney(query.value(15).toDouble()));
      currentSubtotal +=query.value(15).toDouble();
    }

    bool bPrintFooter = !query.next();
    if (!bPrintFooter)
      bPrintFooter = query.value(0).toInt() != currentNumber;
    query.previous();

    if (bPrintFooter)
    {
      if (bHasProduct)
        html += "</table>";
      html += "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">";
      if (query.value(3).toDouble() != 0.0)
      {
        html += QString("<tr><td align=\"right\">Subtotal: %1</td></tr>").arg(Data::strMoney(currentSubtotal));
        html += QString("<tr><td align=\"right\">Desconto: %1</td></tr>").arg(Data::strMoney(query.value(3).toDouble()));
      }
      double total = currentSubtotal + query.value(3).toDouble();
      html += QString("<tr><td align=\"right\">TOTAL: %1</td></tr></table><hr>").arg(Data::strMoney(total));
      currentSubtotal = 0.0;
      currentTotal += total;
    }
  } while (query.next());

  html += QString(
  "<br><table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: black;\">"
    "<tr><th colspan=\"2\">Sumário</th></tr>"
    "<tr><td align=\"center\">Número de compras: %1</td>"
        "<td align=\"center\">Total das compras: %2</td></tr></table></body></html>").arg(Data::strInt(nPurchases),
                                                                                          Data::strMoney(currentTotal));
  return html;
}
