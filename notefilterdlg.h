#ifndef NOTEFILTERWIDGET_H
#define NOTEFILTERWIDGET_H

#include <QDialog>
#include "filterwidget.h"

class JDatabasePicker;
class QDateEdit;
class QCheckBox;

class NoteFilterDlg : public QDialog, FilterWidget
{
  Q_OBJECT

public:
  explicit NoteFilterDlg(QWidget* parent = nullptr);
  virtual QString getFilter() const;
  virtual void clearFilter();

private slots:
  void updateControls();

private:
  QCheckBox* m_cbDate;
  QDateEdit* m_dtBegin;
  QDateEdit* m_dtEnd;
  JDatabasePicker* m_supplierPicker;
  JDatabasePicker* m_productPicker;
};

#endif // NOTEFILTERWIDGET_H
