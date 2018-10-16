#ifndef JITEMTABLE_H
#define JITEMTABLE_H

#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include "jitem.h"

class QKeyEvent;

class JTable : QTableWidget
{
  Q_OBJECT

  struct Columns
  {
    QString m_name;
    QHeaderView::ResizeMode m_resizeMode;
  };

public:
  explicit JTable(QWidget* parent = nullptr);
  virtual const QVector<JItem>& getItems() = 0;

public slots:
  virtual void addItem(const JItem& o) = 0;
  virtual void addItems(const QVector<JItem>& v);
  virtual void removeItem();
  virtual void removeAllItems();
  virtual bool hasItems() const;

protected slots:
  virtual void update(int row, int column) = 0;

private slots:
  void emitChangedSignal();
  virtual void itemDoubleClicked() = 0;

protected:
  void keyPressEvent(QKeyEvent *event);

signals:
  void changedSignal();
};

class JItemTable : QFrame
{
  Q_OBJECT

public:
  explicit JItemTable(QWidget* parent = nullptr);
  virtual const QVector<JItem>& getItems();

public slots:
  virtual void addItem(const JItem& o);
  virtual void removeItem();
  virtual void removeAllItems();
  virtual bool hasItems() const;

protected:
  QTableWidget* m_table;
};

#endif // JITEMTABLE_H
