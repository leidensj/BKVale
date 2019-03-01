#ifndef JDATEPICKER_H
#define JDATEPICKER_H

#include <QWidget>
#include <QDate>

class JDateEdit;
class QPushButton;

class JDatePicker : public QWidget
{
  Q_OBJECT

public:
  explicit JDatePicker(QWidget* parent = nullptr);
  QDate getDate() const;
  void setDate(const QDate& dt);

public slots:
  void checkDate();
  void setToday();

private slots:
  void emitDateChangedSignal();

private:
  JDateEdit* m_dt;
  QPushButton* m_btn;

signals:
  void dateChangedSignal();
};

#endif // JDATEPICKER_H
