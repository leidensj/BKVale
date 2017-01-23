#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "printer.h"

namespace Ui {
class BKVale;
}

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
    Printer m_printer;
    QString m_portName;

private slots:
  void createNewItem();
  void evaluateCellContent(int row, int column);
  void testPrint();
};

#endif // MAINWINDOW_H
