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
  if (m_filter != nullptr)
  {
    QFile data("output.txt");
    if (data.open(QFile::WriteOnly | QFile::Truncate))
    {
      QTextStream out(&data);
      qDebug() << "Inicio: " << QTime::currentTime().toString("hh:mm:ss.zzz");
      QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
      QSqlQuery query(db);
      query.prepare("SELECT _NOTES._NUMBER AS NUMBER, _NOTES._DATE AS _DATE, _NOTES._OBSERVATION AS OBSERVATION, _NOTES._DISCCOUNT AS DISCCOUNT, _NOTES._PAYMENT_METHOD AS PAYMENT,"
                    "SUPPLIERFORM._NAME AS SUPPLIER, STOREFORM._NAME AS STORE,"
                    "_PRODUCTS._NAME AS PRODUCT, _PRODUCTS._UNITY AS UNITY,"
                    "_NOTE_ITEMS._AMMOUNT AS AMMOUNT, _NOTE_ITEMS._PRICE AS PRICE, _NOTE_ITEMS._IS_PACK AS ISPACK, _NOTE_ITEMS._PACK_UNITY AS PACKUNITY, _NOTE_ITEMS._PACK_AMMOUNT AS PACKAMMOUNT,"
                    "_NOTE_ITEMS._PRICE * _NOTE_ITEMS._AMMOUNT AS SUBTOTAL "
                    "FROM _NOTE_ITEMS "
                    "LEFT JOIN _NOTES ON _NOTE_ITEMS._NOTEID = _NOTES._ID "
                    "LEFT JOIN _SUPPLIERS ON _NOTES._SUPPLIERID = _SUPPLIERS._ID "
                    "LEFT JOIN _FORMS AS SUPPLIERFORM ON _SUPPLIERS._FORMID = SUPPLIERFORM._ID "
                    "LEFT JOIN _STORES ON _NOTES._STOREID = _STORES._ID "
                    "LEFT JOIN _FORMS AS STOREFORM ON _STORES._FORMID = STOREFORM._ID "
                    "LEFT JOIN _PRODUCTS ON _NOTE_ITEMS._PRODUCTID = _PRODUCTS._ID");
      if (query.exec())
      {
        int lastNumber = -1;
        while (query.next())
        {
          if (query.value(0).toInt() != lastNumber)
            out << query.value("SUPPLIER").toString() << Qt::endl;
          lastNumber = query.value(0).toInt();
        }
      }
      qDebug() << "Fim: " << QTime::currentTime().toString("hh:mm:ss.zzz");
    }
  }
}
