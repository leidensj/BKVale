#include "purchasereport.h"
#include "filters/purchasefilter.h"
#include "items/purchase.h"
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>

PurchaseReport::PurchaseReport(QWidget* parent)
 : QDialog(parent)
 , m_filter(nullptr)
 , m_btnProcess(nullptr)
 , m_btnPrint(nullptr)
 , m_report(nullptr)
{
  setWindowIcon(QIcon(":/icons/res/purchase.png"));
  setWindowTitle(tr("Relatório de Compras"));

  m_filter = new PurchaseFilter;
  QFrame *line = new QFrame;
  line->setFrameShape(QFrame::HLine);
  m_btnProcess = new QPushButton;
  m_btnProcess->setFlat(true);
  m_btnProcess->setIconSize(QSize(24, 24));
  m_btnProcess->setIcon(QIcon(":/icons/res/process.png"));
  m_btnProcess->setToolTip(tr("Processar"));
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
  QVBoxLayout* ltControls = new QVBoxLayout;
  ltControls->setContentsMargins(0, 0, 0 ,0);
  ltControls->addWidget(m_filter);
  ltControls->addWidget(line);
  ltControls->addLayout(ltButtons);
  QHBoxLayout* ltMain = new QHBoxLayout;
  ltMain->addLayout(ltControls);
  ltMain->addWidget(m_report);

  connect(m_btnProcess, SIGNAL(clicked(bool)), this, SLOT(process()));
  setLayout(ltMain);
}

void PurchaseReport::process()
{
  QVector<Id> ids(Purchase::st_SQL_select_all_purchases(m_filter->getFilter()));
  m_report->clear();
  for (int i = 0; i != ids.size(); ++i)
    m_report->append(QString::number(ids.at(i).get()));
}
