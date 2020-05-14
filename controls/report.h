#ifndef REPORT_H
#define REPORT_H

#include <QWidget>

class QPushButton;
class QTextEdit;
class QDialog;
class PurchaseReport;
class QProgressBar;
class QLabel;

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
  void updateControls();

private:
  QPushButton* m_btnPurchase;
  QPushButton* m_btnPdf;
  QTextEdit* m_report;
  PurchaseReport* m_rptPurchase;
  QDialog* m_dlgPurchase;
  QLabel* m_lblProgress;
  QProgressBar* m_progress;
};

#endif // REPORT_H
