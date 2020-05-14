#ifndef REPORT_H
#define REPORT_H

#include <QWidget>

class QPushButton;
class QTextEdit;
class QDialog;
class PurchaseReport;

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
  void fontSizeChanged();

private:
  QPushButton* m_btnPurchase;
  QPushButton* m_btnPdf;
  QPushButton* m_btnFontSize;
  QTextEdit* m_report;
  PurchaseReport* m_rptPurchase;
  QDialog* m_dlgPurchase;
};

#endif // REPORT_H
