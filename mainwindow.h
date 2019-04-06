#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTcpSocket>
#include "settingsdlg.h"
#include "databaseutils.h"
#include "activeuser.h"

namespace Ui {
class BaitaAssistant;
}

enum class Functionality : int
{
  None = 0,
  Note,
  Reminder,
  Calculator,
  Shop,
  Reservation,
  Discount
};

class QLabel;
class NoteView;
class ReminderView;
class ConsumptionWidget;
class CalculatorWidget;
class ShopView;
class ReservationView;
class DiscountView;
class JMdiSubWindow;

class BaitaAssistant : public QMainWindow
{
  Q_OBJECT

public:
  explicit BaitaAssistant(const ActiveUser& login, QWidget *parent = 0);
  ~BaitaAssistant();

protected:
  void closeEvent(QCloseEvent* event);

private:
  Ui::BaitaAssistant *ui;
  ActiveUser m_login;
  NoteView* m_note;
  ReminderView* m_reminder;
  ConsumptionWidget* m_consumption;
  CalculatorWidget* m_calculator;
  ShopView* m_shop;
  ReservationView* m_reservation;
  DiscountView* m_discount;
  QSerialPort m_printerSerial;
  QTcpSocket m_printerTCP;
  Settings m_settings;
  QLabel* m_statusDatabasePath;
  QLabel* m_statusUserName;
  JMdiSubWindow* m_noteWindow;
  JMdiSubWindow* m_reminderWindow;
  JMdiSubWindow* m_calculatorWindow;
  JMdiSubWindow* m_shopWindow;
  JMdiSubWindow* m_reservationWindow;
  JMdiSubWindow* m_discountWindow;
  bool connectPrinter();
  void disconnectPrinter();
  Functionality getCurrentFunctionality() const;

private slots:
  void updateControls();
  void updateStatusBar();
  void showInfo();
  void print();
  bool print(const QString& text);
  void openSettingsDialog();
  void openUsersDialog();
  void openProductsDialog();
  void openCategoriesDialog();
  void openLoginDialog();
  void openImagesDialog();
  void openFormsDialog();
  void openEmployeesDialog();
  void openShoppingListDialog();
  void openActiveUsersDialog();
  void openStoreDialog();
  void activateWindow();

  void testTimeAccess();
};

#endif // MAINWINDOW_H
