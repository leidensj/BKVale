#ifndef JDATEPICKER_H
#define JDATEPICKER_H

#include <QWidget>
#include <QDate>

class JDateEdit;
class QToolButton;

class JDatePicker : public QWidget
{
  Q_OBJECT

public:
  explicit JDatePicker(QWidget* parent = nullptr);
  QDate getDate() const;
  void setDate(const QDate& dt);

public slots:
  void checkDate();

private:
  JDateEdit* m_dt;
  QToolButton* m_btn;

signals:
  void dateChangedSignal(const QDate& dt);
};

#endif // JDATEPICKER_H
