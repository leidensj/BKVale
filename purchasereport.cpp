#include "purchasereport.h"
#include "filters/purchasefilter.h"
#include "items/purchase.h"
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTextEdit>

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
    double total = 0.0;
    for (int i = 0; i != ids.size(); ++i)
    {
      Purchase o(ids.at(i));
      QString error;
      if (o.SQL_select(error))
      {
        m_report->append(tr("Número: %1").arg(o.strNumber()));
        m_report->append(tr("Loja: %1").arg(o.m_store.name()));
        m_report->append(tr("Fornecedor: %1").arg(o.m_supplier.m_id.isValid()
                                                  ? o.m_supplier.name()
                                                  : tr("Não Informado")));
        m_report->append(tr("Data: %1 %2").arg(o.strDate(), o.strDayOfWeek()));
        m_report->append(tr("Subtotal:  %1").arg(o.strSubTotal()));
        if (o.m_disccount > 0.0)
          m_report->append(tr("%1: %2").arg(o.m_disccount > 0
                                            ? tr("Acréscimos") : tr("Descontos"),
                                            o.strDisccount()));
        m_report->append(tr("Total:  %1").arg(o.strTotal()));
        m_report->append("");
        total += o.total();
      }
      m_report->append(tr("Número de compras: %1").arg(Data::strInt(ids.size())));
      m_report->append(tr("Total das compras: %1").arg(Data::strMoney(total)));
    }
  }
}
