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
  explicit BaitaAssistant(QWidget *parent = 0);
  ~BaitaAssistant();
  void init();

private:
  Ui::BaitaAssistant *ui;
  NoteWidget* m_note;
  ReminderWidget* m_reminder;
  ConsumptionWidget* m_consumption;
  CalculatorWidget* m_calculator;
  QSerialPort m_printer;
  QTcpSocket m_printerTCP;
  quint16 m_nextBlockSizeTCP;
  Settings m_settings;
  QSqlDatabase m_db;

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
  void openItemsDialog();
  void openUsersDialog();
};

#endif // MAINWINDOW_H
