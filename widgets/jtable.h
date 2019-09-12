#ifndef JITEMTABLE_H
#define JITEMTABLE_H

#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include <QDate>
#include "items/jitem.h"

class QKeyEvent;
class QPushButton;

class JTable : private QTableWidget
{
  Q_OBJECT

public:
  explicit JTable(QWidget* parent = nullptr);
  void setLargerSize(bool b);
  bool isValidRow(int row) const;
  bool hasItems() const;
  bool isValidCurrentRow() const;

public slots:
  virtual void removeItem();
  virtual void removeAllItems();

protected slots:
  void emitChangedSignal();

protected:
  void keyPressEvent(QKeyEvent *event);
  void setHeaderIcon(int pos, const QIcon& icon);

signals:
  void changedSignal(bool bIsRowSelected);
};

class JTableItem : public QTableWidgetItem
{
public:
  virtual void evaluate() = 0;
  virtual void erase() = 0;
  virtual void activate() = 0;

private:
  virtual bool evaluate(const QString& exp) = 0;
};

class DoubleItem : public JTableItem
{
public:
  enum class Color
  {
    None,
    Background,
    Foreground
  };

  DoubleItem(Data::Type type,
             Color color,
             bool bCheckable = false,
             bool bAcceptNegative = true,
             const QString& prefix = "",
             const QString& sufix = "");
  void setValue(double val);
  double getValue() const;
  void evaluate();

private:
  bool evaluate(const QString& exp);
  const Data::Type m_type;
  const Color m_color;
  const bool m_bCheckable;
  const bool m_bAcceptNegative;
  const QString m_prefix;
  const QString m_sufix;
};

class DateItem : public JTableItem
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

class TimeItem : public JTableItem
{
public:
  TimeItem(const QTime& defaultTime);
  void setTime(const QTime& t);
  QTime getTime() const;
  void evaluate();

private:
  bool evaluate(const QString& exp);
  QTime m_defaultTime;
};

class TextItem : public JTableItem
{
public:
  TextItem(Text::Input input, bool toUpper = true);
  void evaluate();

private:
  bool evaluate(const QString& exp);
  Text::Input m_input;
  bool m_toUpper;
};

class JItemSQLItem : public JTableItem
{
public:
  JItemSQLItem(const QString& tableName);
  void evaluate();
  Id getId() const;
  void setItem(Id id, const QString& name);

private:
  bool evaluate(const QString& exp);
  const QString& m_tableName;
};

#endif // JITEMTABLE_H
