#ifndef PURCHASEREPORT_H
#define PURCHASEREPORT_H

#include <QWidget>

class QPushButton;
class QTextEdit;
class PurchaseFilter;

class PurchaseReport : public QWidget
{
  Q_OBJECT

public:
  explicit PurchaseReport(PurchaseFilter* filter, QWidget* parent = nullptr);

private slots:
  void process();
  void updateControls();

private:
  const PurchaseFilter* m_filter;
  QPushButton* m_btnProcess;
  QPushButton* m_btnPrint;
  QPushButton* m_btnPdf;
  QTextEdit* m_report;
};

#endif // PURCHASEREPORT_H
