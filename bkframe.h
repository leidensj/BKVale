#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include "cons.h"

namespace Ui {
class BKFrame;
}

class BKFrame : public QFrame
{
  Q_OBJECT

public:
  explicit BKFrame(QWidget *parent = 0);
  ~BKFrame();
  void getContent(TableContent& tableContent, QString& total) const;
  bool isValidSelection() const;

private:
  Ui::BKFrame *ui;
  QString computeUnitValue(int row) const;
  QString computeSubTotal(int row) const;
  QString computeTotal() const;
  double evaluate(int row, int column);
  static QString format(const QString& str, bool b3places = false);
  static QString format(double d, bool b3places = false);

private slots:
  void updateTable(int row, int column);
  void tableSelectionChanged();

public slots:
  void addItem();
  void removeItem();

signals:
  void tableSelectionChangedSignal();

};

#endif // BKFRAME_H
