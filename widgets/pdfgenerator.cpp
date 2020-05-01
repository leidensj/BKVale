#include "pdfgenerator.h"
#include <QTextDocument>
#include <QPrinter>
#include <QDesktopServices>
#include <QThread>

PdfGenerator::PdfGenerator(const QString& fileName,
                           const QString& html,
                           bool bOpen,
                           bool bShowPageNumber)
  : m_fileName(fileName)
  , m_html(html)
  , m_bOpen(bOpen)
  , m_bShowPageNumber(bShowPageNumber)
{

}

void PdfGenerator::work()
{
  QTextDocument doc;
  doc.setHtml(m_html);
  doc.setDocumentMargin(20);
  QPrinter printer(QPrinter::PrinterResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPaperSize(QPrinter::A4);
  printer.setOutputFileName(m_fileName);
  if (!m_bShowPageNumber)
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
  printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
  doc.print(&printer);
  if (m_bOpen)
    QDesktopServices::openUrl(QUrl(m_fileName, QUrl::TolerantMode));
  emit finished();
}

void PdfGenerator::generate()
{
  QThread* thread = new QThread;
  moveToThread(thread);
  connect(thread, SIGNAL(started()), this, SLOT(work()));
  connect(this, SIGNAL(finished()), thread, SLOT(quit()));
  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  thread->start();
}
