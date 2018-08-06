#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTcpSocket>
#include "settingsdlg.h"
#include "databaseutils.h"

namespace Ui {
class BaitaAssistant;
}

class QLabel;
class NoteView;
class ReminderView;
class ConsumptionWidget;
class CalculatorWidget;
class ShopView;
class ReservationView;

enum class Functionality : int
{
  NoteMode = 0,
  ReminderMode,
  CalculatorMode,
  ShopMode,
  ReservationMode
};

class BaitaAssistant : public QMainWindow
{
  Q_OBJECT

public:
  explicit BaitaAssistant(const UserLoginSQL& userLogin, QWidget *parent = 0);
  ~BaitaAssistant();

protected:
  void closeEvent(QCloseEvent* event);

private:
  Ui::BaitaAssistant *ui;
  UserLoginSQL m_userLogin;
  NoteView* m_note;
  ReminderView* m_reminder;
  ConsumptionWidget* m_consumption;
  CalculatorWidget* m_calculator;
  ShopView* m_shop;
  ReservationView* m_reservation;
  QSerialPort m_printerSerial;
  QTcpSocket m_printerTCP;
  Settings m_settings;
  QLabel* m_statusDatabasePath;
  QLabel* m_statusUserName;
  bool connectPrinter();
  void disconnectPrinter();

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
  void openPersonsDialog();
  void openShoppingListDialog();
  void openActiveUsersDialog();
  void openProductBarcodeDialog();
  void reconnectDatabase();
};

#endif // MAINWINDOW_H
