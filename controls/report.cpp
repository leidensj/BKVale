#include "report.h"
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QDate>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMenu>
#include "widgets/filegenerator.h"
#include "widgets/jstatusprogressbarinstance.h"
#include "widgets/jstatusmessageinstance.h"
#include "widgets/findwidget.h"
#include "purchasereport.h"

#define PURCHASE_REPORT tr("Relatório de Compras")
#define FONT_SIZE_PROP "fontsize"

Report::Report(QWidget *parent)
  : QWidget(parent)
  , m_btnPurchase(nullptr)
  , m_btnPdf(nullptr)
  , m_btnFontSize(nullptr)
  , m_report(nullptr)
  , m_rptPurchase(nullptr)
  , m_dlgPurchase(nullptr)
  , m_find(nullptr)
{
  m_btnPurchase = new QPushButton;
  m_btnPurchase->setFlat(true);
  m_btnPurchase->setIconSize(QSize(24, 24));
  m_btnPurchase->setIcon(QIcon(":/icons/res/purchase.png"));
  m_btnPurchase->setToolTip(PURCHASE_REPORT);

  m_btnPdf = new QPushButton;
  m_btnPdf->setFlat(true);
  m_btnPdf->setIconSize(QSize(24, 24));
  m_btnPdf->setIcon(QIcon(":/icons/res/pdf.png"));
  m_btnPdf->setToolTip(tr("Salvar como PDF"));

  m_report = new QTextEdit;
  m_report->setReadOnly(true);

  m_btnFontSize = new QPushButton;
  m_btnFontSize->setFlat(true);
  m_btnFontSize->setIconSize(QSize(24, 24));
  m_btnFontSize->setIcon(QIcon(":/icons/res/text.png"));
  m_btnFontSize->setToolTip(tr("Fonte"));
  m_btnFontSize->setMenu(new QMenu);
  for (int i = 6; i <= 16; i += 2)
  {
    QAction* act = m_btnFontSize->menu()->addAction(QString::number(i), this, SLOT(fontSizeChanged()));
    act->setProperty(FONT_SIZE_PROP, i);
    act->setCheckable(true);
    if (i == 8)
      act->trigger();
  }

  m_rptPurchase = new PurchaseReport;
  QVBoxLayout *ltPurchase = new QVBoxLayout;
  QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  ltPurchase->addWidget(m_rptPurchase);
  ltPurchase->addWidget(btns);

  m_dlgPurchase = new QDialog(this);
  m_dlgPurchase->setLayout(ltPurchase);
  m_dlgPurchase->setWindowFlags(Qt::Window);
  m_dlgPurchase->setWindowTitle(PURCHASE_REPORT);
  m_dlgPurchase->setWindowIcon(QIcon(":/icons/res/purchase.png"));
  m_dlgPurchase->setModal(true);

  m_find = new FindWidget(m_report);

  QHBoxLayout* ltButton = new QHBoxLayout;
  ltButton->setContentsMargins(0, 0, 0, 0);
  ltButton->setAlignment(Qt::AlignLeft);
  ltButton->addWidget(m_btnPurchase);
  ltButton->addWidget(m_btnPdf);
  ltButton->addWidget(m_btnFontSize);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(ltButton);
  ltMain->addWidget(m_find);
  ltMain->addWidget(m_report);

  connect(m_btnPurchase, SIGNAL(clicked(bool)), this, SLOT(openPurchaseReport()));
  connect(m_btnPdf, SIGNAL(clicked(bool)), this, SLOT(toPdf()));
  connect(btns, SIGNAL(accepted()), m_dlgPurchase, SLOT(accept()));
  connect(btns, SIGNAL(rejected()), m_dlgPurchase, SLOT(reject()));

  m_btnPdf->setEnabled(false);
  m_btnFontSize->setEnabled(false);
  setLayout(ltMain);
}

void Report::fontSizeChanged()
{
  if (sender() != nullptr)
  {
    int size = sender()->property(FONT_SIZE_PROP).toInt();
    if (size >= 6 && size <= 16)
    {
      for (auto act : m_btnFontSize->menu()->actions())
        act->setChecked(act == sender());
      QFont f = m_report->font();
      f.setPointSize(size);
      m_report->setFont(f);
    }
  }
}

void Report::print()
{
  QPrinter printer;
  QPrintDialog dlg(&printer, this);
  dlg.setWindowTitle(tr("Imprimir"));
  dlg.setWindowIcon(QIcon(":/icons/res/printer.png"));
  if (m_report->textCursor().hasSelection())
    dlg.setOption(QAbstractPrintDialog::PrintSelection);
  if (dlg.exec() != QDialog::Accepted)
    return;
  m_report->print(&printer);
}

void Report::toPdf()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Salvar relatório"),
                                                  "/desktop/relatorio_compras_" +
                                                  QDate::currentDate().toString("dd_MM_yyyy") +
                                                  ".pdf",
                                                  tr("PDF (*.pdf)"));

  if (fileName.isEmpty())
    return;

  PdfGenerator* w = new PdfGenerator(fileName, m_report->toHtml(), tr("Gerando relatório:"), true, false);
  w->generate();
}

void Report::openPurchaseReport()
{
  if (m_dlgPurchase->exec())
  {
    QString html = m_rptPurchase->process();
    m_report->setHtml(html);
    m_btnPdf->setEnabled(!html.isEmpty());
    m_btnFontSize->setEnabled(!html.isEmpty());
  }
}
