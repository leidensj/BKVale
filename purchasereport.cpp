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
}

QString PurchaseReport::getProductFilter() const
{
  QString str;
  Ids ids(m_productPicker->getIds());
  if (!ids.isEmpty())
  {
    str += " _NOTE_ITEMS._PRODUCTID IN (";
    for (auto id : ids)
      str += id.str() + ",";
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
    for (auto it : vs)
      aux += it + "; ";
    aux.chop(2);
  }
  else
    aux = tr("não especificado");
  str += QString("<tr><td>Fornecedor: %1</td></tr>").arg(aux);

  auto vp = m_productPicker->getNames();
  aux.clear();
  if (!vp.isEmpty())
  {
    for (auto it : vp)
      aux += it + "; ";
    aux.chop(2);
  }
  else
    aux = tr("não especificado");
  str += QString("<tr><td>Produto: %1</td></tr>").arg(aux);

  auto vst = m_storePicker->getNames();
  aux.clear();
  if (!vst.isEmpty())
  {
    for (auto it :vst)
      aux += it + "; ";
    aux.chop(2);
  }
  else
    aux = tr("não especificado");
  str += QString("<tr><td>Loja: %1</td></tr>").arg(aux);

  aux.clear();
  if (!m_cbPaymentCredit->isChecked() && !m_cbPaymentCash->isChecked() && !m_cbPaymentBonus->isChecked())
    aux = tr("não especificado");
  else
  {
    if (m_cbPaymentCredit->isChecked())
      aux += Purchase::st_paymentText(Purchase::PaymentMethod::Credit) + "; ";
    if(m_cbPaymentCash->isChecked())
      aux += Purchase::st_paymentText(Purchase::PaymentMethod::Cash)  + "; ";
    if (m_cbPaymentBonus->isChecked())
      aux += Purchase::st_paymentText(Purchase::PaymentMethod::Bonus)  + "; ";
    aux.chop(2);
  }
  str += QString("<tr><td>Pagamento: %1</td></tr></table>").arg(aux);
  return str;
}

QString PurchaseReport::processProduct() const
{
  QString strQuery = "SELECT " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_NUM " AS NUMBER," //0
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DAT " AS _DATE," //1
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_OBS " AS OBSERVATION," //2
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DIS " AS DISCCOUNT," //3
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_PAY " AS PAYMENT," //4
                     "SUPPLIERFORM." FORM_SQL_COL_NAM " AS SUPPLIER," //5
                     "STOREFORM." FORM_SQL_COL_NAM " AS STORE," //6
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_NAM " AS PRODUCT," //7
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_UNI " AS UNITY," //8
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID " AS PRODUCTID," //9
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_AMM " AS AMMOUNT," //10
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PRI " AS PRICE," //11
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_ISP " AS ISPACK," //12
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PUN " AS PACKUNITY," //13
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PAM " AS PACKAMMOUNT," //14
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PRI " * "
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_AMM " AS SUBTOTAL " //15
                     "FROM " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                     " LEFT JOIN " PURCHASE_SQL_TABLE_NAME " ON "
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_NID " = "
                     PURCHASE_SQL_TABLE_NAME "." SQL_COLID
                     " FULL JOIN " SUPPLIER_SQL_TABLE_NAME " ON "
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SID " = "
                     SUPPLIER_SQL_TABLE_NAME "." SQL_COLID
                     " LEFT JOIN " FORM_SQL_TABLE_NAME " AS SUPPLIERFORM ON "
                     SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL_FID " = "
                     "SUPPLIERFORM." SQL_COLID
                     " LEFT JOIN " STORE_SQL_TABLE_NAME " ON "
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_TID " = "
                     STORE_SQL_TABLE_NAME "." SQL_COLID
                     " LEFT JOIN " FORM_SQL_TABLE_NAME " AS STOREFORM ON "
                     STORE_SQL_TABLE_NAME "." STORE_SQL_COL_FID " = "
                     "STOREFORM." SQL_COLID
                     " LEFT JOIN " PRODUCT_SQL_TABLE_NAME " ON "
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID " = "
                     PRODUCT_SQL_TABLE_NAME "." SQL_COLID;
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
      html += QString("<br><table cellspacing=\"0\" cellpadding=\"3\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: lightgray;\">"
                      "<thead><tr><th align=\"left\" colspan=\"7\">%1</th></tr>").arg(query.value(7).toString());
      html += "<tr><th>Data</th><th>Número</th><th>Fornecedor</th><th>Loja</th><th>Quantidade</th><th>Preço</th><th>Subtotal</th></tr>";
    }
    html += QString(
                "<tr>"
                "<td width=\"7%\">%1</td>"
                "<td width=\"7%\">%2</td>"
                "<td width=\"28%\">%3</td>"
                "<td width=\"28%\">%4</td>"
                "<td width=\"10%\">%5</td>"
                "<td width=\"10%\">%6</td>"
                "<td width=\"10%\">%7</td>"
                "</tr>").arg(query.value(1).toDate().toString("dd/MM/yyyy"),
                             Data::strInt(query.value(0).toLongLong()),
                             query.value(5).toString(),
                             query.value(6).toString(),
                             Data::strAmmount(query.value(10).toDouble()) +
                             (query.value(12).toBool()
                              ? query.value(13).toString()
                              : query.value(8).toString()),
                             Data::strMoney(query.value(11).toDouble()),
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
  QString strQuery = "SELECT " PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_NUM " AS NUMBER," //0
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DAT " AS _DATE," //1
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_OBS " AS OBSERVATION," //2
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_DIS " AS DISCCOUNT," //3
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_PAY " AS PAYMENT," //4
                     "SUPPLIERFORM." FORM_SQL_COL_NAM " AS SUPPLIER," //5
                     "STOREFORM." FORM_SQL_COL_NAM " AS STORE," //6
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_NAM " AS PRODUCT," //7
                     PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_UNI " AS UNITY," //8
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID " AS PRODUCTID," //9
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_AMM " AS AMMOUNT," //10
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PRI " AS PRICE," //11
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_ISP " AS ISPACK," //12
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PUN " AS PACKUNITY," //13
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PAM " AS PACKAMMOUNT," //14
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PRI " * "
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_AMM " AS SUBTOTAL " //15
                     "FROM  " PURCHASE_ELEMENTS_SQL_TABLE_NAME
                     " FULL JOIN " PURCHASE_SQL_TABLE_NAME " ON "
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_NID " = "
                     PURCHASE_SQL_TABLE_NAME "." SQL_COLID
                     " FULL JOIN " SUPPLIER_SQL_TABLE_NAME " ON "
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_SID " = "
                     SUPPLIER_SQL_TABLE_NAME "." SQL_COLID
                     " LEFT JOIN " FORM_SQL_TABLE_NAME " AS SUPPLIERFORM ON "
                     SUPPLIER_SQL_TABLE_NAME "." SUPPLIER_SQL_COL_FID " = "
                     "SUPPLIERFORM." SQL_COLID
                     " LEFT JOIN " STORE_SQL_TABLE_NAME " ON "
                     PURCHASE_SQL_TABLE_NAME "." PURCHASE_SQL_COL_TID " = "
                     "_STORES." SQL_COLID
                     " LEFT JOIN " FORM_SQL_TABLE_NAME " AS STOREFORM ON "
                     STORE_SQL_TABLE_NAME "." STORE_SQL_COL_FID " = "
                     "STOREFORM." SQL_COLID
                     " LEFT JOIN " PRODUCT_SQL_TABLE_NAME " ON "
                     PURCHASE_ELEMENTS_SQL_TABLE_NAME "." PURCHASE_ELEMENTS_SQL_COL_PID " = "
                     PRODUCT_SQL_TABLE_NAME "." SQL_COLID;

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
        "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: lightgray;\">"
          "<tr><th>Quantidade</th><th>Preço</th><th>Produto</th><th>Subtotal</th></tr>";
      }
      currentNumber = query.value(0).toInt();
      nPurchases++;
    }
    if (bHasProduct)
    {
      html += QString(
      "<tr>"
        "<td width=\"18%\">%1</td>"
        "<td width=\"16%\">%2</td>"
        "<td width=\"50%\">%3</td>"
        "<td width=\"16%\">%4</td>"
      "</tr>").arg(Data::strAmmount(query.value(10).toDouble()) +
                   (query.value(12).toBool()
                   ? query.value(13).toString()
                   : query.value(8).toString()),
                   Data::strMoney(query.value(11).toDouble()),
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
  "<br><table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: lightgray;\">"
    "<tr><th colspan=\"2\">Sumário</th></tr>"
    "<tr><td align=\"center\">Número de compras: %1</td>"
        "<td align=\"center\">Total das compras: %2</td></tr></table></body></html>").arg(Data::strInt(nPurchases),
                                                                                          Data::strMoney(currentTotal));
  return html;
}
