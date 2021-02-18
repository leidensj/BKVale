#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTcpSocket>
#include "settingsdialog.h"
#include "databaseutils.h"
#include "items/login.h"
#include <QMdiArea>
#include <QPainter>

namespace Ui {
class Baita;
}

class JMdiArea : public QMdiArea
{
public:
    JMdiArea(QWidget *parent = nullptr)
      : QMdiArea(parent)
      , m_pixmap(":/icons/res/baitabanner.png")
    {
      setBackground(QBrush(Qt::white));
      QFont f = font();
      f.setPointSize(12);
      setFont(f);
    }
protected:
    void paintEvent(QPaintEvent *event)
    {
        QMdiArea::paintEvent(event);
        QPainter painter(viewport());
        int x = width()/2 - m_pixmap.width()/2;
        int y = height()/2 - m_pixmap.height()/2;
        painter.drawPixmap(x, y, m_pixmap);
    }
private:
    QPixmap m_pixmap;
};

enum class Functionality : int
{
  None = 0,
  Purchase,
  Reminder,
  Calculator,
  Shop,
  Report
};

class QLabel;
class PurchaseView;
class ReminderView;
class ConsumptionWidget;
class CalculatorWidget;
class ShopWidget;
class JMdiSubWindow;
class Report;

class Baita : public QMainWindow
{
  Q_OBJECT

public:
  explicit Baita(QWidget *parent = 0);
  ~Baita();

protected:
  void closeEvent(QCloseEvent* event);

private:
  Ui::Baita *ui;
  JMdiArea* m_mdi;
  PurchaseView* m_purchase;
  Report* m_report;
  ReminderView* m_reminder;
  ConsumptionWidget* m_consumption;
  CalculatorWidget* m_calculator;
  ShopWidget* m_shop;
  QSerialPort m_printerSerial;
  QTcpSocket m_printerTCP;
  Settings m_settings;
  QLabel* m_statusDatabasePath;
  QLabel* m_statusUserName;
  QLabel* m_statusTime;
  JMdiSubWindow* m_purchaseWindow;
  JMdiSubWindow* m_reportWindow;
  JMdiSubWindow* m_reminderWindow;
  JMdiSubWindow* m_calculatorWindow;
  JMdiSubWindow* m_shopWindow;
  bool connectPrinter();
  void disconnectPrinter();
  Functionality getCurrentFunctionality() const;

private slots:
  void updateControls();
  void updateStatusBar();
  void updateTime();
  void showInfo();
  void print();
  bool print(const QString& text);
  void openJItemSQLDialog();
  void openSettingsDialog();
  void openLoginDialog();
  void openLoggedDialog();
  void activateWindow();
  void testTimeAccess();
};

#endif // MAINWINDOW_H
