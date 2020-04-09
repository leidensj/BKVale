#include "purchasereport.h"
#include "filters/purchasefilter.h"
#include "items/purchase.h"
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressDialog>
#include <qtrpt.h>

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

void PurchaseReport::process()
{
  if (m_filter != nullptr)
  {
    QVector<Id> ids(Purchase::st_SQL_select_all_purchases(m_filter->getFilter()));
    m_report->clear();
    if (ids.size() == 0)
      return;

    QtRPT* rpt = new QtRPT(this);
    rpt->loadReport(":/reportsxml/purchase.xml");
    connect(rpt, &QtRPT::setDSInfo, [ids](DataSetInfo& ds)
    {
      ds.recordCount = ids.size();
    });
    connect(rpt, &QtRPT::setValue, [ids](const int recNo, const QString paramName, QVariant &paramValue, const int /*reportPage*/)
    {
      Purchase o(ids.at(recNo));
      QString error;
      if (o.SQL_select(error))
      {
        if (paramName == "supplier")
          paramValue = o.m_supplier.m_form.name();
      }
    });
    rpt->printExec(true);

    QProgressDialog progress("Gerando relatório...", "Abortar", 0, ids.size(), this);
    progress.setWindowIcon(QIcon(":/icons/res/process.png"));
    progress.setWindowTitle(tr("Relatório"));
    progress.setMinimumDuration(500);
    progress.setWindowModality(Qt::WindowModal);
    double total = 0.0;
    int i = 0;
    for (i = 0; i != ids.size(); ++i)
    {
      Purchase o(ids.at(i));
      QString error;
      if (o.SQL_select(error))
      {
        QString str(tr("Número: %1\n"
                       "Loja: %2\n"
                       "Fornecedor: %3\n"
                       "Data: %4 %5\n"
                       "Subtotal: %6\n"
                       "%7%8"
                       "Total: %9\n").arg(o.strNumber(),
                                           o.m_store.name(),
                                           o.m_supplier.m_id.isValid()
                                           ? o.m_supplier.name()
                                           : tr("Não Informado"),
                                           o.strDate(),
                                           o.strDayOfWeek(),
                                           o.strSubTotal(),
                                           o.m_disccount > 0
                                           ? tr("Acréscimos: ")
                                           : o.m_disccount < 0 ? tr("Descontos: ") : "",
                                           o.m_disccount != 0
                                           ? o.strDisccount() + "\n"
                                           : "",
                                           o.strTotal()));
        m_report->append(str);
        total += o.total();
      }
      progress.setValue(i);
      if (progress.wasCanceled())
        break;
    }
    if (i != ids.size())
      i--;
    progress.setValue(ids.size());
    m_report->append(tr("Número de compras: %1").arg(Data::strInt(i)));
    m_report->append(tr("Total das compras: %1").arg(Data::strMoney(total)));
  }
}
