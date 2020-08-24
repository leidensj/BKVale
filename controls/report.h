#ifndef REPORT_H
#define REPORT_H

#include <QWidget>

class QPushButton;
class QTextEdit;
class QDialog;
class PurchaseReport;
class FindWidget;

class Report : public QWidget
{
  Q_OBJECT

public:
  explicit Report(QWidget* parent = nullptr);

public slots:
  void print();
  void toPdf();

private slots:
  void openPurchaseReport();
  void fontSizeChanged();

private:
  QPushButton* m_btnPurchase;
  QPushButton* m_btnPdf;
  QPushButton* m_btnFontSize;
  QTextEdit* m_report;
  PurchaseReport* m_rptPurchase;
  QDialog* m_dlgPurchase;
  FindWidget* m_find;
};

#endif // REPORT_H
