#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QDate>

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
  QSerialPort m_printer;
  QDate m_date;
  void updateUI();

private slots:
  void createNewItem();
  void evaluateCellContent(int row, int column);
  void connect();
  void disconnect();
  void print();
  void showCalendar();
  void showSettings();
};

#endif // MAINWINDOW_H
