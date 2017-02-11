#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"

namespace Ui {
class BKVale;
}

class BKVale : public QMainWindow
{
  Q_OBJECT

public:
  explicit BKVale(QWidget *parent = 0);
  ~BKVale();

private:
  Ui::BKVale *ui;
  QSerialPort m_printer;
  Settings m_settings;
  void setItemEditable(int row, int column, bool editable);

private slots:
  void connect();
  void disconnect();
  void print();
  void showSettings();
  void enableControls();
};

#endif // MAINWINDOW_H
