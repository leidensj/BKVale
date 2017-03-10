#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"
#include "databaseutils.h"
#include "notewidget.h"

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
  NoteWidget* m_noteWidget;
  QSerialPort m_printer;
  Settings m_settings;
  Database m_db;

private slots:
  void connect();
  void disconnect();
  void print();
  void showSettings();
  void enableControls();
  void createNew();
};

#endif // MAINWINDOW_H
