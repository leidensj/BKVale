#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsdialog.h"
#include "databaseutils.h"
#include "items/login.h"
#include <QMdiArea>
#include <QPainter>
#include <QMap>

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

class QLabel;
class PurchaseView;
class ReminderView;
class ConsumptionWidget;
class CalculatorWidget;
class ShopWidget;
class JMdiSubWindow;
class Report;
class CouponRedeemer;

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
  Settings m_settings;
  QLabel* m_statusDatabasePath;
  QLabel* m_statusUserName;
  QLabel* m_statusTime;
  JMdiSubWindow* m_purchaseWindow;
  JMdiSubWindow* m_reportWindow;
  JMdiSubWindow* m_reminderWindow;
  JMdiSubWindow* m_calculatorWindow;
  JMdiSubWindow* m_shopWindow;
  CouponRedeemer* m_redeemer;
  QMap<Functionality::Idx, QAction*> m_actions;
  Functionality::Idx getCurrentFunctionality() const;

private slots:
  void updateControls();
  void updateStatusBar();
  void updateTime();
  void openJItemSQLDialog();
  void openSettingsDialog();
  void openLoginDialog();
  void openLoggedDialog();
  void activateWindow();
  void testTimeAccess();
  void about();
};

#endif // MAINWINDOW_H
