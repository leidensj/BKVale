#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

#include <QObject>

class FileGenerator : public QObject
{
  Q_OBJECT

public:
  FileGenerator(const QString& fileName,
                const QString& text,
                const QString& statusMessage,
                int flags);

public slots:
  void generate();
  virtual void work() = 0;

signals:
  void finished();

protected:
  QString m_fileName;
  QString m_text;
  QString m_statusMessage;
  int m_flags;
};

class PdfGenerator : public FileGenerator
{
  Q_OBJECT

public:
  PdfGenerator(const QString& fileName,
               const QString& html,
               const QString& statusMessage,
               bool bOpen,
               bool bShowPageNumber);

public slots:
  void work();
};

class CsvGenerator : public FileGenerator
{
  Q_OBJECT

public:
  CsvGenerator(const QString& fileName,
               const QString& text,
               const QString& statusMessage,
               bool bOpen);

public slots:
  void work();
};

#endif // FILEGENERATOR_H
