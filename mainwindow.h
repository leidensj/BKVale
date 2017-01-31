#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class BKVale;
}

class QComboBox;

enum Columns
{
  Ammount,
  Unity,
  Description,
  UnitValue,
  TotalValue
};

class BKVale : public QMainWindow
{
  Q_OBJECT

public:
  explicit BKVale(QWidget *parent = 0);
  ~BKVale();

private:
  Ui::BKVale *ui;
  QString m_portName;
  QComboBox* m_availablePorts;
  QSerialPort m_printer;
  void updateUI();

private slots:
  void createNewItem();
  void evaluateCellContent(int row, int column);
  void refreshAvailablePorts();
  void connect();
  void disconnect();
  void print();
  void showCalendar();
};

#endif // MAINWINDOW_H
