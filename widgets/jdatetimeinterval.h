#ifndef JDATETIMEINTERVAL_H
#define JDATETIMEINTERVAL_H

#include <QWidget>
#include <QDateTime>
#include <QGroupBox>

class JDateEdit;
class JTimeEdit;
class QToolButton;

class JDateTimeInterval : public QGroupBox
{
  Q_OBJECT

public:
  explicit JDateTimeInterval(QWidget *parent = nullptr);
  QDateTime getInitialDateTime() const;
  QDateTime getFinalDateTime() const;
  void clear();

public slots:
  void year();
  void month();
  void week();
  void day();
  void lastYear();
  void lastWeek();
  void lastMonth();
  void yesterday();

signals:
  void changedSignal();

private slots:
  void emitChangedSignal();

private:
  QToolButton* m_btn;
  JDateEdit* m_dtInit;
  JTimeEdit* m_tmInit;
  JDateEdit* m_dtFinal;
  JTimeEdit* m_tmFinal;
};

#endif
