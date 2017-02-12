#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <vector>

namespace Ui {
class BKFrame;
}

enum class Column : int
{
  Ammount,
  Unity,
  Description,
  UnitValue,
  SubTotal
};

typedef std::vector<std::vector<QString>> TableContent;

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
  void updateTable();

public slots:
  void addItem();
  void removeItem();
};

#endif // BKFRAME_H
