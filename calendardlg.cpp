#include "calendardlg.h"
#include "ui_calendardlg.h"

CalendarDlg::CalendarDlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CalendarDlg)
{
  ui->setupUi(this);
  QObject::connect(ui->calendarWidget,
                   SIGNAL(activated(const QDate&)),
                   this,
                   SLOT(updateDate(const QDate&)));
}

CalendarDlg::~CalendarDlg()
{
  delete ui;
}

const QDate& CalendarDlg::getDate()
{
  return m_date;
}

void CalendarDlg::updateDate(const QDate & date)
{
  m_date = date;
  accept();
}
