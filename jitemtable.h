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
  explicit JTable(QWidget* parent = nullptr);
  virtual const JItem& getItem(int row) const = 0;
  bool isValidRow(int row) const;

public slots:
  virtual void addItem(const JItem& o) = 0;
  virtual void removeItem();
  virtual void removeAllItems();
  virtual bool hasItems() const;

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
