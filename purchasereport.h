#ifndef PURCHASEREPORT_H
#define PURCHASEREPORT_H

#include <QWidget>
#include <QSqlQuery>

class QPushButton;
class QTextEdit;
class PurchaseFilter;
class QRadioButton;

class PurchaseReport : public QWidget
{
  Q_OBJECT

public:
  explicit PurchaseReport(PurchaseFilter* filter, QWidget* parent = nullptr);

private slots:
  void fetch();
  void process();
  void processPurchase();
  void processProductBegin();
  void processProduct();
  void updateControls();
  void saveAsPdf();
  void print();

private:
  QSqlQuery m_query;
  QString m_html;
  const PurchaseFilter* m_filter;
  QPushButton* m_btnProcess;
  QPushButton* m_btnPrint;
  QPushButton* m_btnPdf;
  QTextEdit* m_report;
  QRadioButton* m_rdoPurchase;
  QRadioButton* m_rdoProduct;
  QRadioButton* m_rdoSupplier;
  QString getQuery() const;
};

#endif // PURCHASEREPORT_H
