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
      QVector<Id> ids(Purchase::st_SQL_select_all_purchases(m_filter->getFilter()));
      Purchase o;
      QString error;
      QTextStream out(&data);
      qDebug() << "Size: " << QString::number(ids.size());
      qDebug() << "Inicio: " << QTime::currentTime().toString("hh:mm:ss.zzz");
      QSqlDatabase db(QSqlDatabase::database(POSTGRE_CONNECTION_NAME));
      QSqlQuery query(db);
      for (int i = 0; i != ids.size(); ++i)
      {
        o.m_id = ids.at(i);
        if (o.SQL_select_proc(query, error))
          out << o.strNumber() << " " << o.m_supplier.name() << Qt::endl;
      }
      qDebug() << "Fim: " << QTime::currentTime().toString("hh:mm:ss.zzz");
    }
  }
}
