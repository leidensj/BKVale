#ifndef REMINDERVIEW_H
#define REMINDERVIEW_H

#include <QFrame>
#include "reminder.h"

class QLineEdit;
class QPlainTextEdit;
class QCheckBox;
class QRadioButton;
class QPushButton;

class ReminderView : public QFrame
{
  Q_OBJECT

public:
  explicit ReminderView(QWidget *parent = 0);
  ~ReminderView();
  Reminder reminder() const;
  bool isValid() const;
  bool isSaveChecked() const;

private slots:
  void emitChangedSignal();
  void emitSearchClickedSignal();
  void setCapitalization(int state);

public slots:
  void setReminder(const Reminder r);
  void clear();

signals:
  changedSignal();
  searchClickedSignal();

private:
  int m_currentID;
  QLineEdit* m_title;
  QPlainTextEdit* m_message;
  QCheckBox* m_capitalization;
  QRadioButton* m_size1;
  QRadioButton* m_size2;
  QPushButton* m_create;
  QPushButton* m_search;
  QCheckBox* m_save;
};

#endif // REMINDERVIEW_H
