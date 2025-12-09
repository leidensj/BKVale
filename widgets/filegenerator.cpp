#include "filegenerator.h"
#include <QTextDocument>
#include <QPrinter>
#include <QDesktopServices>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include "jstatusmessageinstance.h"
#include "jstatusprogressbarinstance.h"

FileGenerator::FileGenerator(const QString& fileName,
                             const QString& text,
                             const QString& statusMessage,
                             int flags)
  : m_fileName(fileName)
  , m_text(text)
  , m_statusMessage(statusMessage)
  , m_flags(flags)
{

}

void FileGenerator::generate()
{
  auto p = JStatusProgressBarInstance::getInstance();
  auto l = JStatusMessageInstance::getInstance();
  if (p != nullptr && l != nullptr)
  {
    p->show();
    l->show();
    l->setText(m_statusMessage);
    connect(this, SIGNAL(finished()), p, SLOT(hide()));
    connect(this, SIGNAL(finished()), l, SLOT(hide()));
  }

  QThread* thread = new QThread;
  moveToThread(thread);
  connect(thread, SIGNAL(started()), this, SLOT(work()));
  connect(this, SIGNAL(finished()), thread, SLOT(quit()));
  connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  thread->start();
}

PdfGenerator:: PdfGenerator(const QString& fileName,
                            const QString& html,
                            const QString& statusMessage,
                            bool bOpen,
                            bool bShowPageNumber)
  : FileGenerator(fileName, html, statusMessage, 0)
{
  m_flags = bOpen ? m_flags | 0x1 : m_flags;
  m_flags = bShowPageNumber ? m_flags | 0x2 : m_flags;
}

void PdfGenerator::work()
{
  QTextDocument doc;
  doc.setHtml(m_text);
  doc.setDocumentMargin(20);
  QPrinter printer(QPrinter::PrinterResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPageSize(QPageSize::A4);
  printer.setOutputFileName(m_fileName);
  QSizeF rect;
  rect.setWidth(printer.width()*1.1);
  rect.setHeight(printer.height()*1.1);
  if (!(m_flags & 0x2))
    doc.setPageSize(rect); // This is necessary if you want to hide the page number
  printer.setPageMargins(QMarginsF(10, 10, 10, 10));
  doc.print(&printer);
  if (m_flags & 0x1)
    QDesktopServices::openUrl(QUrl(m_fileName, QUrl::TolerantMode));
  emit finished();
}

CsvGenerator:: CsvGenerator(const QString& fileName,
                            const QString& html,
                            const QString& statusMessage,
                            bool bOpen)
  : FileGenerator(fileName, html, statusMessage, 0)
{
  m_flags = bOpen ? m_flags | 0x1 : m_flags;
}

void CsvGenerator::work()
{
  QFile file(m_fileName);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
       return;
  QTextStream out(&file);
  out.setEncoding(QStringConverter::Utf8);
  out.setGenerateByteOrderMark(true);
  out << m_text;
  if (m_flags & 0x1)
    QDesktopServices::openUrl(QUrl(m_fileName, QUrl::TolerantMode));
  emit finished();
}


