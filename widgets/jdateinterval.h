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
  void changedSignal(bool bChecked,const QDate& dti, const QDate& dtf);

private slots:
  void emitChangedSignal();

private:
  QToolButton* m_btn;
  JDateEdit* m_dtInit;
  JDateEdit* m_dtFinal;
};

#endif
