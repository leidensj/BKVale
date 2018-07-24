#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QString>
#include <QDialog>

class FilterDlg : public QDialog
{
  Q_OBJECT

public:

  explicit FilterDlg(QWidget* parent = nullptr)
    : QDialog(parent)
  {

  }

  virtual QString getFilter() const = 0;
  virtual void clearFilter() = 0;
};

#endif // FILTERWIDGET_H
