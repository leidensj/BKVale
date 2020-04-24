#include "purchasereport.h"
#include "filters/purchasefilter.h"
#include "items/purchase.h"
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressDialog>
#include "reportcore.h"
#include "reportinterface.h"
#include "reportpreview.h"
#include "qtrpt.h"

PurchaseReport::PurchaseReport(PurchaseFilter* filter, QWidget* parent)
 : QWidget(parent)
 , m_filter(filter)
 , m_btnProcess(nullptr)
 , m_btnPrint(nullptr)
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
  m_report = new QTextEdit;
  m_report->setReadOnly(true);

  QHBoxLayout* ltButtons= new QHBoxLayout;
  ltButtons->setContentsMargins(0, 0, 0 ,0);
  ltButtons->setAlignment(Qt::AlignLeft);
  ltButtons->addWidget(m_btnProcess);
  ltButtons->addWidget(m_btnPrint);
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
                "_NOTE_ITEMS._AMMOUNT AS AMMOUNT," //9
                "_NOTE_ITEMS._PRICE AS PRICE," //10
                "_NOTE_ITEMS._IS_PACK AS ISPACK," //11
                "_NOTE_ITEMS._PACK_UNITY AS PACKUNITY," //12
                "_NOTE_ITEMS._PACK_AMMOUNT AS PACKAMMOUNT," //13
                "_NOTE_ITEMS._PRICE * _NOTE_ITEMS._AMMOUNT AS SUBTOTAL " //14
                "FROM _NOTE_ITEMS "
                "LEFT JOIN _NOTES ON _NOTE_ITEMS._NOTEID = _NOTES._ID "
                "LEFT JOIN _SUPPLIERS ON _NOTES._SUPPLIERID = _SUPPLIERS._ID "
                "LEFT JOIN _FORMS AS SUPPLIERFORM ON _SUPPLIERS._FORMID = SUPPLIERFORM._ID "
                "LEFT JOIN _STORES ON _NOTES._STOREID = _STORES._ID "
                "LEFT JOIN _FORMS AS STOREFORM ON _STORES._FORMID = STOREFORM._ID "
                "LEFT JOIN _PRODUCTS ON _NOTE_ITEMS._PRODUCTID = _PRODUCTS._ID");
  if (query.exec() && query.next())
  {
    auto r = new QtRPT(this);
    r->loadReport(":/reportsxml/purchase.xml");
    connect(r, &QtRPT::setValue,
            [&query](const int recNo, const QString paramName, QVariant &paramValue, const int /*reportPage*/)
    {
      qDebug() << "recNo " <<recNo << " query.at() " << query.at();
      if (recNo == (query.at() + 1))
        query.next();
      else if (recNo == (query.at() - 1))
        query.previous();

      if (query.at() == recNo)
      {
        if (paramName == "number")
        {
          paramValue = query.value(0);
        }
        else if(paramName == "_date")
        {
          paramValue = query.value(1).toDate().toString("dd/MM/yyyy dddd");
        }
        else if(paramName == "payment")
        {
          paramValue = Purchase::st_paymentText((Purchase::PaymentMethod)query.value(4).toInt());
        }
        else if(paramName == "supplier")
        {
          paramValue = query.value(5);
        }
        else if(paramName == "store")
        {
          paramValue = query.value(6);
        }
        else if(paramName == "product")
        {
          paramValue = query.value(7);
        }
        else if(paramName == "ammount")
        {
          paramValue = Data::strAmmount(query.value(9).toDouble()) +
                       (query.value(11).toBool()
                        ? query.value(12).toString()
                        : query.value(8).toString()) +
                       " x R$" + Data::strMoney(query.value(10).toDouble());
        }
        else if(paramName == "subtotal")
        {
          paramValue = query.value(14);
        }
        else if(paramName == "disccount")
        {
          paramValue = query.value(3);
        }
      }
    });
    int count = query.size();
    connect(r, &QtRPT::setDSInfo,[count](DataSetInfo& ds)
    {
      if (ds.reportPage == 0)
        ds.recordCount = count;
    });
    r->printExec();
  }
}
