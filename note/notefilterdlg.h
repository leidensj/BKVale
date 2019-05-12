#ifndef NOTEFILTERDLG_H
#define NOTEFILTERDLG_H

#include "filterdlg.h"

class JDatabasePicker;
class QDateEdit;
class QCheckBox;

class NoteFilterDlg : public FilterDlg
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

#endif // NOTEFILTERDLG_H
