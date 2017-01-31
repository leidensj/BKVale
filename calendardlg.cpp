#include "calendardlg.h"
#include "ui_calendardlg.h"

CalendarDlg::CalendarDlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CalendarDlg)
{
  ui->setupUi(this);
}

CalendarDlg::~CalendarDlg()
{
  delete ui;
}
