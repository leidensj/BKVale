#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"
#include "databaseutils.h"
#include "cons.h"

namespace Ui {
class BKVale;
}

class BKFrame;

class BKVale : public QMainWindow
{
  Q_OBJECT

public:
  explicit BKVale(QWidget *parent = 0);
  ~BKVale();

private:
  Ui::BKVale *ui;
  BKFrame* m_bkframe;
  QSerialPort m_printer;
  Settings m_settings;
  Database m_db;
  PromissoryNote buildPromissoryNote();

private slots:
  void connect();
  void disconnect();
  void print();
  void showSettings();
  void enableControls();
  void showInfo();
};

#endif // MAINWINDOW_H
