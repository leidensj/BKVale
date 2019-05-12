#ifndef JITEMTABLE_H
#define JITEMTABLE_H

#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include <QDate>
#include "jitem.h"

class QKeyEvent;
class QPushButton;

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
  bool isValidRow(int row) const;
  bool hasItems() const;
  bool isValidCurrentRow() const;

public slots:
  virtual void removeItem();
  virtual void removeAllItems();

protected slots:
  void emitChangedSignal();
  void emitDeleteSignal(int row, int column);
  void emitActivateSignal(int row, int column);

protected:
  void keyPressEvent(QKeyEvent *event);

signals:
  void changedSignal();
  void deleteSignal(int row, int column);
  void activateSignal(int row, int column);
};

class JItemTable : public JTable
{
  Q_OBJECT

public:
  explicit JItemTable(int flags = 0, QWidget* parent = nullptr);
  virtual const JItem& getItem(int row) const = 0;

public slots:
  virtual void addItem(const JItem& o) = 0;

protected slots:
  virtual void update(int row, int column) = 0;
  virtual void itemActivate(int row, int column) = 0;
  virtual void itemDelete(int row, int column) = 0;

protected:
  void setHeaderIcon(int pos, const QIcon& icon);
  void setHeaderIconSearchable(int pos);
};

class JTableButtons : public QWidget
{
  Q_OBJECT

public:
  explicit JTableButtons(JTable* table, int maxNumberofItems = 100, int orientation = Qt::Vertical, QWidget* parent = nullptr);

private slots:
  void updateControls();

private:
  JTable* m_table;
  int m_max;
  QPushButton* m_btnAdd;
  QPushButton* m_btnRemove;
};

class DoubleItem : public QTableWidgetItem
{
public:
  enum class Color
  {
    None,
    Background,
    Foreground
  };

  DoubleItem(JItem::DataType type, Color color, bool bCheckable = false);
  void setValue(double val);
  double getValue() const;
  void evaluate();

private:
  bool evaluate(const QString& exp);
  const JItem::DataType m_type;
  const Color m_color;
  const bool m_bCheckable;
};

class DateItem : public QTableWidgetItem
{
public:
  enum class Color
  {
    None,
    DateBeforeDefault
  };

  DateItem(const QDate& defaultDate, Color color = Color::None);
  void setDate(const QDate& dt);
  QDate getDate() const;
  void evaluate();

private:
  bool evaluate(const QString& exp);
  QDate m_defaultDate;
  Color m_color;
};

#endif // JITEMTABLE_H
