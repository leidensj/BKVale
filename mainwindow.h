#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"
#include "databaseutils.h"
#include "notewidget.h"
#include "reminderwidget.h"
#include "consumptionwidget.h"

namespace Ui {
class BaitaAssistant;
}

class NoteWidget;

enum class Functionality : int
{
  NoteMode = 0,
  ReminderMode,
  ShopMode,
  ConsumptionMode
};

class BaitaAssistant : public QMainWindow
{
  Q_OBJECT

public:
  explicit BaitaAssistant(QWidget *parent = 0);
  ~BaitaAssistant();

private:
  Ui::BaitaAssistant *ui;
  NoteWidget m_note;
  ReminderWidget m_reminder;
  ConsumptionWidget m_consumption;
  QSerialPort m_printer;
  Settings m_settings;
  bool m_bReady;
  QSqlDatabase m_db;

private slots:
  void connect();
  void disconnect();
  void showSettings();
  void enableControls();
  void init();
  void showInfo();
  void notePrint();
  void openItemsDialog();

signals:
  void initSignal();
};

#endif // MAINWINDOW_H
