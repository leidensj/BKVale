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
  ConsumptionMode,
  ReservationMode
};

class BaitaAssistant : public QMainWindow
{
  Q_OBJECT

public:
  explicit BaitaAssistant(const UserLoginSQL& userLogin, QWidget *parent = 0);
  ~BaitaAssistant();

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

private slots:
  void connect();
  void connectedTCP();
  void disconnect();
  void updateControls();
  void updateStatusBar();
  void showInfo();
  void print();
  void print(const QString& text);
  void openSettingsDialog();
  void openUsersDialog();
  void openProductsDialog();
  void openCategoriesDialog();
  void openLoginDialog();
  void openImagesDialog();
  void openPersonsDialog();
  void openShoppingListDialog();
};

#endif // MAINWINDOW_H
