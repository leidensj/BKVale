#ifndef REMINDERVIEW_H
#define REMINDERVIEW_H

#include <QFrame>
#include "items/reminder.h"
#include "widgets/jitemview.h"
#include <QDialog>

class JLineEdit;
class QPlainTextEdit;
class QCheckBox;
class QRadioButton;
class JSpinBox;

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
  void emitChangedSignal();
  void setCapitalization(int state);

signals:
  void changedSignal();

private:
  JLineEdit* m_edTitle;
  QPlainTextEdit* m_teMessage;
  JLineEdit* m_edBarcode;
  QCheckBox* m_cbCapitalization;
  QRadioButton* m_rdSize1;
  QRadioButton* m_rdSize2;
  QCheckBox* m_cbFavorite;
  QCheckBox* m_cbBarcodeHRI;
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
