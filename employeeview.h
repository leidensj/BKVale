#ifndef EMPLOYEEVIEW_H
#define EMPLOYEEVIEW_H

#include "employee.h"
#include "jitemview.h"
#include <QVector>

class QPushButton;
class JLineEdit;
class JDatabasePicker;
class QCheckBox;
class TimeIntervalWidget;
class JTimeEdit;
class JSpinBox;
class QLabel;

class TimeIntervalWidget : public QWidget
{
  Q_OBJECT

  JTimeEdit* m_tmBegin[10];
  JTimeEdit* m_tmEnd[10];
  JSpinBox* m_spn;

public:
  explicit TimeIntervalWidget(QWidget* parent = nullptr);
  void setItems(const QVector<TimeInterval>& v);
  QVector<TimeInterval> getItems() const;

private slots:
  void validate();
  void adjustControls();
};

class EmployeeView : public JItemView
{
  Q_OBJECT

public:
  explicit EmployeeView(QWidget* parent = 0);
  const JItemSQL& getItem() const;

public slots:
  void create();

private:
  mutable Employee m_ref;
  JDatabasePicker* m_formPicker;
  JDatabasePicker* m_storePicker;
  JLineEdit* m_edPincode;
  QCheckBox* m_cbNoteEdit;
  QCheckBox* m_cbNoteRemove;
  TimeIntervalWidget* m_time;
  void setItem(const JItemSQL& o);
};

#endif // EMPLOYEEVIEW_H
