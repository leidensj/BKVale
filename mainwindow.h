#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"
#include "databaseutils.h"
#include "notewidget.h"
#include "reminderwidget.h"

namespace Ui {
class BaitaAssistant;
}

class NoteWidget;

enum class Functionality : int
{
    NoteMode = 0,
    ReminderMode,
    ShopMode
};

class BaitaAssistant : public QMainWindow
{
  Q_OBJECT

public:
  explicit BaitaAssistant(QWidget *parent = 0);
  ~BaitaAssistant();

private:
  Ui::BaitaAssistant *ui;
  NoteWidget m_noteWidget;
  ReminderWidget m_reminderWidget;
  QSerialPort m_printer;
  Settings m_settings;
  Database m_db;
  bool m_bReady;

private slots:
  void connect();
  void disconnect();
  void showSettings();
  void enableControls();
  void init();
  void showInfo();
  void notePrint();

signals:
  void initSignal();
};

#endif // MAINWINDOW_H
