#include "purchasereport.h"
#include "filters/purchasefilter.h"
#include "items/purchase.h"
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressDialog>
#include <QScrollBar>

PurchaseReport::PurchaseReport(PurchaseFilter* filter, QWidget* parent)
 : QWidget(parent)
 , m_filter(filter)
 , m_btnProcess(nullptr)
 , m_btnPrint(nullptr)
 , m_btnPdf(nullptr)
 , m_report(nullptr)
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

  QHBoxLayout* ltButtons= new QHBoxLayout;
  ltButtons->setContentsMargins(0, 0, 0 ,0);
  ltButtons->setAlignment(Qt::AlignLeft);
  ltButtons->addWidget(m_btnProcess);
  ltButtons->addWidget(m_btnPrint);
  ltButtons->addWidget(m_btnPdf);
  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(ltButtons);
  ltMain->addWidget(m_report);

  connect(m_btnProcess, SIGNAL(clicked(bool)), this, SLOT(process()));
  connect(m_report, SIGNAL(textChanged()), this, SLOT(updateControls()));

  setLayout(ltMain);
  updateControls();
}

void PurchaseReport::updateControls()
{
  m_btnPrint->setEnabled(!m_report->document()->isEmpty());
}

#include <qdebug.h>

void PurchaseReport::process()
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
  strQuery += " ORDER BY NUMBER ASC";
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
      html += "<html><body><h1 align=\"center\">Relatório de Compras</h1>";
      scrollValue = m_report->verticalScrollBar()->value();
    }

    while (query.next())
    {
      if (currentNumber != query.value(0).toInt())
      {
        html += QString(
        "<br><table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\">"
          "<tr>"
            "<td width=\"30%\">Número: %1</td>"
            "<td width=\"30%\">Data: %2</td>"
            "<td width=\"40%\">Pagamento: %3</td>"
          "</tr>"
          "<tr>"
            "<td width=\"50%\">Fornecedor: %4</td>"
            "<td width=\"50%\">Loja: %5</td>"
          "</tr>"
        "</table>").arg(Data::strInt(query.value(0).toInt()),
                        query.value(1).toDate().toString("dd/MM/yyyy dddd"),
                        Purchase::st_paymentText((Purchase::PaymentMethod)query.value(4).toInt()),
                        query.value(5).toString(),
                        query.value(6).toString());
        if (query.value(9).toInt() != 0) // hasProduct
        {
          html +=
          "<table cellspacing=\"0\" cellpadding=\"1\" align=\"center\" width=\"100%\" style=\"border-width: 1px;border-style: solid;border-color: gray;\">"
            "<tr>"
              "<th>Quantidade</th>"
              "<th>Produto</th>"
              "<th>Subtotal</th>"
            "</tr>";
        }
        currentNumber = query.value(0).toInt();
        nPurchases++;
      }
      if (query.value(9).toInt() != 0) // hasProduct
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
        if (query.value(9).toInt() != 0) // hasProduct
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
      html += QString("<h3>Número de compras: %1</h3>").arg(Data::strInt(nPurchases));
      html += QString("<h3>Total das compras: %1</h3>").arg(Data::strMoney(currentTotal));
      html += "</body></html>";
      scrollValue = m_report->verticalScrollBar()->value();
      m_report->setHtml(html);
      m_report->verticalScrollBar()->setValue(scrollValue);
    }
  }
}
