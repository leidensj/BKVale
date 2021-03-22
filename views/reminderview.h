#ifndef REMINDERVIEW_H
#define REMINDERVIEW_H

#include <QFrame>
#include "items/reminder.h"
#include "jitemview.h"
#include <QDialog>

class JLineEdit;
class QPlainTextEdit;
class QCheckBox;
class JSpinBox;
class JDatePicker;
class QComboBox;
class JTimeEdit;

class ReminderView : public JItemView
{
  Q_OBJECT

public:
  explicit ReminderView(QWidget *parent = 0);

public slots:
  void getItem(JItemSQL& o) const;

protected slots:
  void setItem(const JItemSQL& o);

private slots:
  void updateControls();
  void setFavorite();
  void print();

private:
  JLineEdit* m_edTitle;
  QPlainTextEdit* m_teMessage;
  JLineEdit* m_edBarcode;
  QCheckBox* m_cbCapitalization;
  QCheckBox* m_cbSize;
  QCheckBox* m_cbFavorite;
  QCheckBox* m_cbBarcodeHRI;
  QComboBox* m_subject;
  JDatePicker* m_date;
  QCheckBox* m_cbDate;
  JTimeEdit* m_time;
  QCheckBox* m_cbTime;
  QPushButton* m_btnPin;
};

#endif // REMINDERVIEW_H
