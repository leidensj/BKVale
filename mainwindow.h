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

class NoteWidget;
class ReminderWidget;
class ConsumptionWidget;
class CalculatorWidget;

enum class Functionality : int
{
  NoteMode = 0,
  ReminderMode,
  CalculatorMode,
  ShopMode,
  ConsumptionMode
};

class BaitaAssistant : public QMainWindow
{
  Q_OBJECT

public:
  explicit BaitaAssistant(const UserLoginSQL& userLogin, QWidget *parent = 0);
  void init();
  ~BaitaAssistant();

private:
  Ui::BaitaAssistant *ui;
  UserLoginSQL m_userLogin;
  NoteWidget* m_note;
  ReminderWidget* m_reminder;
  ConsumptionWidget* m_consumption;
  CalculatorWidget* m_calculator;
  QSerialPort m_printer;
  QTcpSocket m_printerTCP;
  quint16 m_nextBlockSizeTCP;
  Settings m_settings;

private slots:
  void connect();
  void connectTCP();
  void connectedTCP();
  void disconnect();
  void showSettings();
  void enableControls();
  void showInfo();
  void print();
  bool print(const QString& text);
  void printFullCut();
  void openUsersDialog();
  void openItemsDialog();
  void openLoginDialog();
};

#endif // MAINWINDOW_H
