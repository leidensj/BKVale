#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"
<<<<<<< HEAD
#include "databaseutils.h"
#include "promissorynotewidget.h"
=======
>>>>>>> parent of da3042d... criando banco de dados

namespace Ui {
class BKVale;
}

class PromissoryNoteWidget;

class BKVale : public QMainWindow
{
  Q_OBJECT

public:
  explicit BKVale(QWidget *parent = 0);
  ~BKVale();

private:
  Ui::BKVale *ui;
  PromissoryNoteWidget* m_promissoryNoteWidget;
  QSerialPort m_printer;
  Settings m_settings;
<<<<<<< HEAD
  Database m_db;
=======
>>>>>>> parent of da3042d... criando banco de dados

private slots:
  void connect();
  void disconnect();
  void print();
  void showSettings();
  void enableControls();
};

#endif // MAINWINDOW_H
