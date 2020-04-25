#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include <QObject>

class QThread;

class PdfGenerator : public QObject
{
  Q_OBJECT

public:
  PdfGenerator(const QString& fileName,
               const QString& html,
               bool bOpen,
               bool bShowPageNumber);

public slots:
  void generate();
  void work();

signals:
  void finished();

private:
  QString m_fileName;
  QString m_html;
  bool m_bOpen;
  bool m_bShowPageNumber;
};

#endif // PDFGENERATOR_H
