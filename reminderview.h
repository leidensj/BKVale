#ifndef REMINDERVIEW_H
#define REMINDERVIEW_H

#include <QFrame>
#include "items/reminder.h"
#include <QDialog>

class JLineEdit;
class QPlainTextEdit;
class QCheckBox;
class QRadioButton;
class QPushButton;
class QDockWidget;
class JDatabase;
class JSpinBox;

class ReminderView : public QFrame
{
  Q_OBJECT

public:
  explicit ReminderView(QWidget *parent = 0);
  Reminder getReminder() const;
  void setReminder(const Reminder& reminder);
  bool save();

private slots:
  void emitChangedSignal();
  void setCapitalization(int state);
  void search();
  void itemSelected(const JItemSQL& jItem);
  void itemsRemoved(const QVector<Id>& ids);

public slots:
  void create();

signals:
  void changedSignal();

private:
  Id m_currentId;
  JLineEdit* m_edTitle;
  QPlainTextEdit* m_teMessage;
  JLineEdit* m_edBarcode;
  QCheckBox* m_cbCapitalization;
  QRadioButton* m_rdSize1;
  QRadioButton* m_rdSize2;
  QCheckBox* m_cbFavorite;
  QCheckBox* m_cbBarcodeHRI;
  QPushButton* m_btnCreate;
  QPushButton* m_btnSearch;
  JDatabase* m_database;
  QDockWidget* m_dock;
};

class ReminderPrintDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ReminderPrintDialog(QWidget* parent = nullptr);
  int getCopies() const;
  bool getSave() const;

private:
  JSpinBox* m_spnCopies;
  QCheckBox* m_cbSave;
};

#endif // REMINDERVIEW_H
