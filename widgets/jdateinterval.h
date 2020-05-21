#ifndef JDATEINTERVAL_H
#define JDATEINTERVAL_H

#include <QWidget>
#include <QDate>
#include <QGroupBox>

class JDateEdit;
class QToolButton;

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
  void initialDateChangedSignal(const QDate& dt);
  void finalDateChangedSignal(const QDate& dt);

private:
  QToolButton* m_btn;
  JDateEdit* m_dtInit;
  JDateEdit* m_dtFinal;
};

#endif
