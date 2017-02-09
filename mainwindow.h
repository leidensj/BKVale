#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "settingsdlg.h"

namespace Ui {
class BKVale;
}

class QComboBox;

enum TableColumn
{
  TableColumnAmmount,
  TableColumnUnity,
  TableColumnDescription,
  TableColumnUnitValue,
  TableColumnSubTotalValue
};

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
  void enableControls();
  double total() const;

private slots:
  void addItem();
  void removeItem();
  void evaluateCellContent(int row, int column);
  void connect();
  void disconnect();
  void print();
  void showSettings();
  void updateUI();
};

#endif // MAINWINDOW_H
