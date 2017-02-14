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
  void getContent(TableContent& tableContent, QString& total);

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

public slots:
  void addItem();
  void removeItem();
};

#endif // BKFRAME_H
