#ifndef TIMEINTERVALDLG_H
#define TIMEINTERVALDLG_H

#include <QDialog>
#include <QVector>
#include <timeinterval.h>

class JSpinBox;
class QDialogButtonBox;
class JTimeEdit;

class TimeIntervalDlg : public QDialog
{
  Q_OBJECT

public:
  explicit TimeIntervalDlg(QWidget* parent = nullptr);
  void setItems(const QVector<TimeInterval>& v);
  const QVector<TimeInterval>& get() const;

private slots:
  void validate();
  void adjustControls();

private:
  JTimeEdit* m_tmBegin[10];
  JTimeEdit* m_tmEnd[10];
  JSpinBox* m_spn;
  QDialogButtonBox* m_btn;
};

#endif // TIMEINTERVALDLG_H
