#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTcpSocket>
#include "settingsdlg.h"
#include "databaseutils.h"
#include "items/activeuser.h"
#include <QMdiArea>
#include <QPainter>

namespace Ui {
class Tipi;
}

class JMdiArea : public QMdiArea
{
public:
    JMdiArea(QWidget *parent = nullptr)
      : QMdiArea(parent)
      , m_pixmap(":/icons/res/tipi.png")
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
  Shop
};

class QLabel;
class PurchaseView;
class ReminderView;
class ConsumptionWidget;
class CalculatorWidget;
class ShopView;
class JMdiSubWindow;

class Tipi : public QMainWindow
{
  Q_OBJECT

public:
  explicit Tipi(const ActiveUser& login, QWidget *parent = 0);
  ~Tipi();

protected:
  void closeEvent(QCloseEvent* event);

private:
  Ui::Tipi *ui;
  ActiveUser m_login;
  JMdiArea* m_mdi;
  PurchaseView* m_purchase;
  ReminderView* m_reminder;
  ConsumptionWidget* m_consumption;
  CalculatorWidget* m_calculator;
  ShopView* m_shop;
  QSerialPort m_printerSerial;
  QTcpSocket m_printerTCP;
  Settings m_settings;
  QLabel* m_statusDatabasePath;
  QLabel* m_statusUserName;
  JMdiSubWindow* m_purchaseWindow;
  JMdiSubWindow* m_reminderWindow;
  JMdiSubWindow* m_calculatorWindow;
  JMdiSubWindow* m_shopWindow;
  bool connectPrinter();
  void disconnectPrinter();
  Functionality getCurrentFunctionality() const;

private slots:
  void updateControls();
  void updateStatusBar();
  void showInfo();
  void print();
  bool print(const QString& text);
  void openJItemSQLDialog();
  void openSettingsDialog();
  void openLoginDialog();
  void openActiveUsersDialog();
  void activateWindow();
  void testTimeAccess();
};

#endif // MAINWINDOW_H
