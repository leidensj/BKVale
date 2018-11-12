#ifndef JITEMTABLE_H
#define JITEMTABLE_H

#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include "jitem.h"

class QKeyEvent;

class JTable : public QTableWidget
{
  Q_OBJECT

public:

  enum class Flags
  {
    NoFlags = 0x0,
    BigFont = 0x1,
    Uppercase = 0x2
  };

  explicit JTable(int flags = 0, QWidget* parent = nullptr);
  virtual const JItem& getItem(int row) const = 0;
  bool isValidRow(int row) const;
  bool hasItems() const;
  bool isValidCurrentRow() const;

public slots:
  virtual void addItem(const JItem& o) = 0;
  virtual void removeItem();
  virtual void removeAllItems();

protected slots:
  virtual void update(int row, int column) = 0;
  virtual void itemActivate(int row, int column) = 0;
  virtual void itemDelete(int row, int column) = 0;
  void emitChangedSignal();

protected:
  void keyPressEvent(QKeyEvent *event);

signals:
  void changedSignal();
  void deleteSignal(int row, int column);
  void activateSignal(int row, int column);
};

#endif // JITEMTABLE_H
