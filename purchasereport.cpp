#include "purchasereport.h"
#include "filters/purchasefilter.h"
#include "items/purchase.h"
#include "widgets/pdfgenerator.h"
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressDialog>
#include <QScrollBar>
#include <QFileDialog>
#include <QRadioButton>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>

PurchaseReport::PurchaseReport(PurchaseFilter* filter, QWidget* parent)
 : QWidget(parent)
 , m_query(QSqlDatabase::database(POSTGRE_CONNECTION_NAME))
 , m_filter(filter)
 , m_btnProcess(nullptr)
 , m_btnPrint(nullptr)
 , m_btnPdf(nullptr)
 , m_report(nullptr)
 , m_rdoPurchase(nullptr)
 , m_rdoProduct(nullptr)
 , m_rdoSupplier(nullptr)
{
  m_btnProcess = new QPushButton;
  m_btnProcess->setFlat(true);
  m_btnProcess->setIconSize(QSize(24, 24));
  m_btnProcess->setIcon(QIcon(":/icons/res/process.png"));
  m_btnProcess->setToolTip(tr("Gerar"));
  m_btnPrint = new QPushButton;
  m_btnPrint->setFlat(true);
  m_btnPrint->setIconSize(QSize(24, 24));
  m_btnPrint->setIcon(QIcon(":/icons/res/printer.png"));
  m_btnPrint->setToolTip(tr("Imprimir"));
  m_btnPdf = new QPushButton;
  m_btnPdf->setFlat(true);
  m_btnPdf->setIconSize(QSize(24, 24));
  m_btnPdf->setIcon(QIcon(":/icons/res/pdf.png"));
  m_btnPdf->setToolTip(tr("Salvar como PDF"));
  m_report = new QTextEdit;
  m_report->setReadOnly(true);
  m_rdoPurchase = new QRadioButton(tr("Compra"));
  m_rdoPurchase->setIcon(QIcon(":/icons/res/purchase.png"));
  m_rdoPurchase->setChecked(true);
  m_rdoProduct = new QRadioButton(tr("Produto"));
  m_rdoProduct->setIcon(QIcon(":/icons/res/item.png"));
  m_rdoSupplier = new QRadioButton(tr("Fornecedor"));
  m_rdoSupplier->setIcon(QIcon(":/icons/res/supplier.png"));

  QFrame* vFrame1 = new QFrame;
  vFrame1->setFrameShape(QFrame::VLine);

  QHBoxLayout* ltButtons= new QHBoxLayout;
  ltButtons->setContentsMargins(0, 0, 0 ,0);
  ltButtons->setAlignment(Qt::AlignLeft);
  ltButtons->addWidget(m_btnProcess);
  ltButtons->addWidget(m_btnPrint);
  ltButtons->addWidget(m_btnPdf);
  ltButtons->addWidget(vFrame1);
  ltButtons->addWidget(new QLabel(tr("Agrupar:")));
  ltButtons->addWidget(m_rdoPurchase);
  ltButtons->addWidget(m_rdoProduct);
  ltButtons->addWidget(m_rdoSupplier);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(ltButtons);
  ltMain->addWidget(m_report);

  connect(m_btnProcess, SIGNAL(clicked(bool)), this, SLOT(process()));
  connect(m_report, SIGNAL(textChanged()), this, SLOT(updateControls()));
  connect(m_btnPdf, SIGNAL(clicked(bool)), this, SLOT(saveAsPdf()));
  connect(m_btnPrint, SIGNAL(clicked(bool)), this, SLOT(print()));
  connect(m_report->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(fetch()));

  setLayout(ltMain);
  updateControls();
}

void PurchaseReport::updateControls()
{
  m_btnPrint->setEnabled(!m_report->document()->isEmpty());
  m_btnPdf->setEnabled(!m_report->document()->isEmpty());
}

void PurchaseReport::print()
{
  QPrinter printer;
  QPrintDialog dlg(&printer, this);
  dlg.setWindowTitle(tr("Imprimir"));
  dlg.setWindowIcon(QIcon(":/icons/res/printer.png"));
  if (m_report->textCursor().hasSelection())
    dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
  if (dlg.exec() != QDialog::Accepted)
    return;
  m_report->print(&printer);
}

void PurchaseReport::saveAsPdf()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Salvar relatório"),
                                                  "/desktop/relatorio_compras_" +
                                                  QDate::currentDate().toString("dd_MM_yyyy") +
                                                  ".pdf",
                                                  tr("PDF (*.pdf)"));

  if (fileName.isEmpty())
    return;

  PdfGenerator* w = new PdfGenerator(fileName, m_report->toHtml(), true, true);
  w->generate();
}

void PurchaseReport::process()
{
  if (m_rdoPurchase->isChecked())
    processPurchase();
  else if (m_rdoProduct->isChecked())
    processProductBegin();
}

void PurchaseReport::processProductBegin()
{
  m_query.clear();
  m_report->clear();
  m_html.clear();
  if (m_filter == nullptr)
    return;

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
  if (!m_filter->getFilter().isEmpty())
    strQuery += " WHERE " + m_filter->getFilter();
  strQuery += " ORDER BY PRODUCT ASC, _DATE DESC, SUPPLIER ASC, NUMBER DESC";
  m_query.prepare(strQuery);
  if (m_query.exec(strQuery) && m_query.next())
  {
    m_html += "<html><body><h1 align=\"center\">Relatório de Compras</h1>";
    processProduct();
  }
}

void PurchaseReport::processProduct()
{
  if (!m_query.isValid())
    return;

  int scrollValue = m_report->verticalScrollBar()->value();
  qlonglong currentProduct = -1;
  double currentSubtotal = 0.0;
  double currentAmmount = 0.0;
  int nEntries = 0;
  do
  {
    if (currentProduct != m_query.value(9).toLongLong())
    {
      currentProduct = m_query.value(9).toLongLong();
      m_html += QString("<br><table cellspacing=\"0\" cellpadding=\"3\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: black;\">"
                        "<thead><tr><th colspan=\"6\">Produto: %1</th></tr>").arg(m_query.value(7).toString());
      m_html += "<tr><th>Data</th><th>Número</th><th>Fornecedor</th><th>Loja</th><th>Quantidade</th><th>Subtotal</th></tr>";
    }
    m_html += QString(
                "<tr>"
                "<td width=\"7%\">%1</td>"
                "<td width=\"7%\">%2</td>"
                "<td width=\"28%\">%3</td>"
                "<td width=\"28%\">%4</td>"
                "<td width=\"20%\">%5</td>"
                "<td width=\"10%\">%6</td>"
                "</tr>").arg(m_query.value(1).toDate().toString("dd/MM/yyyy"),
                             Data::strInt(m_query.value(0).toLongLong()),
                             m_query.value(5).toString(),
                             m_query.value(6).toString(),
                             Data::strAmmount(m_query.value(10).toDouble()) +
                             (m_query.value(12).toBool()
                              ? m_query.value(13).toString()
                              : m_query.value(8).toString()) +
                             " x " + Data::strMoney(m_query.value(11).toDouble()),
                             Data::strMoney(m_query.value(15).toDouble()));
    currentSubtotal += m_query.value(15).toDouble();
    double ammount = m_query.value(10).toDouble();
    if (m_query.value(12).toBool()) // isPack
      ammount *= m_query.value(14).toDouble();
    currentAmmount += ammount;

    bool bHasNext = m_query.next();
    bool bPrintFooter = !bHasNext || currentProduct != m_query.value(9).toLongLong();
    m_query.previous();

    if (bPrintFooter)
    {
      m_html += "</table><table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">";
      m_html += QString(
                  "<tr><th align=\"right\">Sumário do Produto: %1</td></tr>"
                  "<tr><td align=\"right\">Quantidade: %2</td></tr>"
                  "<tr><td align=\"right\">Média de preço: %3</td></tr>"
                  "<tr><td align=\"right\">Total: %4</td></tr></table>").arg(m_query.value(7).toString(),
                                                                             Data::strAmmount(currentAmmount) + m_query.value(8).toString(),
                                                                             Data::strMoney(currentSubtotal / currentAmmount),
                                                                             Data::strMoney(currentSubtotal));
      currentSubtotal = 0.0;
      currentAmmount = 0.0;
    }
    else if (nEntries == 100)
      nEntries--;
  } while (m_query.next() && nEntries++ < 100);

  if (!m_html.isEmpty())
  {
    m_report->verticalScrollBar()->blockSignals(true);
    m_report->setHtml(m_html + "<hr></body></html>");
    m_report->verticalScrollBar()->setValue(scrollValue);
    m_report->verticalScrollBar()->blockSignals(false);
  }
}

void PurchaseReport::fetch()
{
  if (m_rdoProduct->isChecked())
    if (m_report->verticalScrollBar()->value() == m_report->verticalScrollBar()->maximum())
      processProduct();
}

void PurchaseReport::processPurchase()
{
  if (m_filter == nullptr)
    return;

  m_report->clear();
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  QSqlQuery query(db);
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
  if (!m_filter->getFilter().isEmpty())
    strQuery += " WHERE " + m_filter->getFilter();
  strQuery += " ORDER BY NUMBER DESC";
  query.prepare(strQuery);
  if (query.exec())
  {
    int currentNumber = -1;
    double currentSubtotal = 0.0;
    double currentTotal = 0.0;
    int nPurchases = 0;
    int scrollValue = 0;
    QProgressDialog progress(tr("Gerando Relatório"), tr("Cancelar"), 0, query.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    QString html;
    if (query.size() != 0)
    {
      html += "<html><body><h1 align=\"center\">Relatório de Compras</h1><br>";
      scrollValue = m_report->verticalScrollBar()->value();
    }

    while (query.next())
    {
      const bool bHasProduct = query.value(9).toInt() != 0 || query.value(15).toDouble() != 0.0;
      if (currentNumber != query.value(0).toInt())
      {
        html += QString(
        "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">"
          "<thead><tr>"
            "<td width=\"30%\">Número: %1</td>"
            "<td width=\"30%\">Data: %2</td>"
            "<td width=\"40%\">Pagamento: %3</td>"
          "</thead></tr>"
        "</table>"
        "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">"
          "<tr>"
            "<td width=\"50%\">Fornecedor: %4</td>"
            "<td width=\"50%\">Loja: %5</td>"
          "</tr>"
        "</table>").arg(Data::strInt(query.value(0).toInt()),
                        query.value(1).toDate().toString("dd/MM/yyyy dddd"),
                        Purchase::st_paymentText((Purchase::PaymentMethod)query.value(4).toInt()),
                        query.value(5).toString(),
                        query.value(6).toString());
        if (bHasProduct)
        {
          html +=
          "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: black;\">"
            "<tr>"
              "<th>Quantidade</th>"
              "<th>Produto</th>"
              "<th>Subtotal</th>"
            "</tr>";
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
      progress.setValue(query.at());
      if (progress.wasCanceled())
          break;
    }
    progress.setValue(query.size());
    if (query.size() != 0)
    {
      html += QString(
      "<br><table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: black;\">"
        "<tr><th colspan=\"2\">Sumário</th></tr>"
        "<tr><td align=\"center\">Número de compras: %1</td>"
            "<td align=\"center\">Total das compras: %2</td></tr></table></body></html>").arg(Data::strInt(nPurchases), Data::strMoney(currentTotal));
      scrollValue = m_report->verticalScrollBar()->value();
      m_report->setHtml(html);
      m_report->verticalScrollBar()->setValue(scrollValue);
    }
  }

  updateControls();
}
