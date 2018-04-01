#ifndef REMINDERVIEW_H
#define REMINDERVIEW_H

#include <QFrame>
#include <QSqlDatabase>
#include "reminder.h"
#include "settings.h"

class JLineEdit;
class QPlainTextEdit;
class QCheckBox;
class QRadioButton;
class QPushButton;
class QDockWidget;
class JDatabase;

class ReminderView : public QFrame
{
  Q_OBJECT

public:
  explicit ReminderView(QWidget *parent = 0);
  Reminder getReminder() const;
  void setReminder(const Reminder& reminder);
  void setDatabase(QSqlDatabase db);
  void saveAndPrint(QIODevice* printer, InterfaceType type);


private slots:
  void emitChangedSignal();
  void setCapitalization(int state);
  void search();
  void itemSelected(const JItem& jItem);
  void itemRemoved(qlonglong id);
  bool save(const Reminder& reminder);
  bool print(const Reminder& reminder,
             QIODevice* printer,
             InterfaceType type);

public slots:
  void create();

signals:
  void changedSignal();

private:
  int m_currentId;
  JLineEdit* m_edTitle;
  QPlainTextEdit* m_teMessage;
  QCheckBox* m_cbCapitalization;
  QRadioButton* m_rdSize1;
  QRadioButton* m_rdSize2;
  QCheckBox* m_cbFavorite;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSearch;
  QCheckBox* m_cbSave;
  JDatabase* m_database;
  QDockWidget* m_dock;
};

#endif // REMINDERVIEW_H
