#ifndef JDATEINTERVAL_H
#define JDATEINTERVAL_H

#include <QWidget>
#include <QDate>
#include <QGroupBox>

class JDateEdit;
class QPushButton;

class JDateInterval : public QGroupBox
{
  Q_OBJECT

public:
  explicit JDateInterval(QWidget *parent = nullptr);
  QDate getInitialDate() const;
  QDate getFinalDate() const;

public slots:
  void year();
  void month();
  void week();
  void day();

signals:
  void initialDateChanged(const QDate& dt);
  void finalDateChanged(const QDate& dt);

private:
  QPushButton* m_btnYear;
  QPushButton* m_btnMonth;
  QPushButton* m_btnWeek;
  QPushButton* m_btnDay;
  JDateEdit* m_dtInit;
  JDateEdit* m_dtFinal;
};

#endif
