#include "purchasereport.h"
#include "filters/purchasefilter.h"
#include "items/purchase.h"
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressDialog>

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

#include <QTextStream>
#include <qdebug.h>

void PurchaseReport::process()
{
  if (m_filter == nullptr)
    return;

  m_report->clear();
  QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
  QSqlQuery query(db);
  query.prepare("SELECT _NOTES._NUMBER AS NUMBER," //0
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
                "LEFT JOIN _SUPPLIERS ON _NOTES._SUPPLIERID = _SUPPLIERS._ID "
                "LEFT JOIN _FORMS AS SUPPLIERFORM ON _SUPPLIERS._FORMID = SUPPLIERFORM._ID "
                "LEFT JOIN _STORES ON _NOTES._STOREID = _STORES._ID "
                "LEFT JOIN _FORMS AS STOREFORM ON _STORES._FORMID = STOREFORM._ID "
                "LEFT JOIN _PRODUCTS ON _NOTE_ITEMS._PRODUCTID = _PRODUCTS._ID");
  if (query.exec())
  {
    int currentNumber = -1;
    double currentSubtotal = 0.0;
    QProgressDialog progress(tr("Gerando Relatório"), tr("Cancelar"), 0, query.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    if (query.size() != 0)
      m_report->append("<html><body><h1>Relatório de Compras</h1></body></html>");

    while (query.next())
    {
      if (currentNumber != query.value(0).toInt())
      {
        m_report->append(tr("Número: ") + Data::strInt(query.value(0).toInt()));
        m_report->append(tr("Data: ") + query.value(1).toDate().toString("dd/MM/yyyy dddd"));
        m_report->append(tr("Pagamento: ") + Purchase::st_paymentText((Purchase::PaymentMethod)query.value(4).toInt()));
        m_report->append(tr("Fornecedor: ") + query.value(5).toString());
        m_report->append(tr("Loja: ") + query.value(6).toString());
        currentNumber = query.value(0).toInt();
      }
      if (query.value(9).toInt() != 0)
      {
        m_report->append(Data::strAmmount(query.value(10).toDouble()) +
                         (query.value(12).toBool()
                          ? query.value(13).toString()
                          : query.value(8).toString()) +
                         " x " + Data::strMoney(query.value(11).toDouble()) +
                         " " + query.value(7).toString() +
                         " " + Data::strMoney(query.value(15).toDouble()));
        currentSubtotal +=query.value(15).toDouble();
      }
      bool bPrintFooter = !query.next();
      if (!bPrintFooter)
      {
        bPrintFooter = query.value(0).toInt() != currentNumber;
        query.previous();
      }
      if (bPrintFooter)
      {
        if (query.value(3).toDouble() != 0.0)
        {
          m_report->append(tr("Subtotal: ") + Data::strMoney(currentSubtotal));
          m_report->append(tr("Desconto: ") + Data::strMoney(query.value(3).toDouble()));
        }
        m_report->append(tr("TOTAL: ") + Data::strMoney(currentSubtotal + query.value(3).toDouble()));
        currentSubtotal = 0.0;
      }
      progress.setValue(query.at());
      if (progress.wasCanceled())
          break;
    }
    progress.setValue(query.size());
  }
}
