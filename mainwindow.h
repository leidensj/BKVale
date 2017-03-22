#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"
#include "databaseutils.h"
#include "notewidget.h"
#include "historywidget.h"

namespace Ui {
class BKVale;
}

class NoteWidget;

class BKVale : public QMainWindow
{
  Q_OBJECT

public:
  explicit BKVale(QWidget *parent = 0);
  ~BKVale();

private:
  Ui::BKVale *ui;
  NoteWidget m_noteWidget;
  HistoryWidget m_historyWidget;
  QSerialPort m_printer;
  Settings m_settings;
  Database m_db;
  bool m_bReady;
  bool initDatabase(QString& error);

private slots:
  void connect();
  void disconnect();
  void print();
  void showSettings();
  void enableControls();
  void createNew();
  void showInfo();
  void showSearch();
  void openNote(int idx);
};

#endif // MAINWINDOW_H
