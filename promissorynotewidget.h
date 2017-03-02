#ifndef BKFRAME_H
#define BKFRAME_H

#include <QFrame>
#include <QStringList>
#include "common.h"

namespace Ui {
class PromissoryNoteWidget;
}



class PromissoryNoteWidget : public QFrame
{
  Q_OBJECT

public:
  explicit PromissoryNoteWidget(QWidget *parent = 0);
  ~PromissoryNoteWidget();
  bool isValidSelection() const;
  QString serializeTable() const;
  void deserializeTable(const QString& str);
  void setSupplier(const QString& supplier);
  QString getSupplier() const;
  void setNumber(int number);
  int getNumber() const;
  void setDate(const QDate& date);
  QDate getDate() const;
  QString getTableText(int row, Column column) const;
  QString getTotal() const;
  int getTableCount() const;

private:
  static const QChar st_separator;
  Ui::PromissoryNoteWidget *ui;
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
