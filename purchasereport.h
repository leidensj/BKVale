#ifndef PURCHASEREPORT_H
#define PURCHASEREPORT_H

#include <QDialog>

class PurchaseFilter;
class QPushButton;
class QTextEdit;

class PurchaseReport : public QDialog
{
  Q_OBJECT

public:
  explicit PurchaseReport(QWidget* parent = nullptr);

private slots:
  void process();

private:
  PurchaseFilter* m_filter;
  QPushButton* m_btnProcess;
  QPushButton* m_btnPrint;
  QTextEdit* m_report;
};

#endif // PURCHASEREPORT_H
