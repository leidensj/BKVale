#include "report.h"
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QDate>
#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QDialogButtonBox>
#include "widgets/pdfgenerator.h"
#include "purchasereport.h"

#define PURCHASE_REPORT tr("Relatório de Compras")

Report::Report(QWidget *parent)
  : QWidget(parent)
  , m_btnPurchase(nullptr)
  , m_btnPdf(nullptr)
  , m_report(nullptr)
  , m_rptPurchase(nullptr)
  , m_dlgPurchase(nullptr)
  , m_lblProgress(nullptr)
  , m_progress(nullptr)
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

  QHBoxLayout* ltButton = new QHBoxLayout;
  ltButton->setContentsMargins(0, 0, 0, 0);
  ltButton->setAlignment(Qt::AlignLeft);
  ltButton->addWidget(m_btnPurchase);
  ltButton->addWidget(m_btnPdf);

  m_lblProgress = new QLabel(tr("Salvando PDF"));
  m_lblProgress->hide();
  m_progress = new QProgressBar;
  m_progress->setRange(0, 0);
  m_progress->hide();

  QHBoxLayout* ltProgress = new QHBoxLayout;
  ltProgress->setContentsMargins(0, 0, 0, 0);
  ltProgress->addWidget(m_lblProgress);
  ltProgress->addWidget(m_progress);

  QVBoxLayout* ltMain = new QVBoxLayout;
  ltMain->addLayout(ltButton);
  ltMain->addWidget(m_report);
  ltMain->addLayout(ltProgress);
  setLayout(ltMain);

  connect(m_btnPurchase, SIGNAL(clicked(bool)), this, SLOT(openPurchaseReport()));
  connect(m_btnPdf, SIGNAL(clicked(bool)), this, SLOT(toPdf()));
  connect(btns, SIGNAL(accepted()), m_dlgPurchase, SLOT(accept()));
  connect(btns, SIGNAL(rejected()), m_dlgPurchase, SLOT(reject()));
}

void Report::print()
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

  m_lblProgress->show();
  m_progress->show();
  PdfGenerator* w = new PdfGenerator(fileName, m_report->toHtml(), true, true);
  connect(w, SIGNAL(finished()), m_lblProgress, SLOT(hide()));
  connect(w, SIGNAL(finished()), m_progress, SLOT(hide()));
  w->generate();
}

void Report::updateControls()
{
  m_btnPdf->setEnabled(!m_report->toHtml().isEmpty());
}

void Report::openPurchaseReport()
{
  if (m_dlgPurchase->exec())
    m_report->setHtml(m_rptPurchase->process());
  updateControls();
}
