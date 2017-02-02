#include "calendardlg.h"
#include "ui_calendardlg.h"

CalendarDlg::CalendarDlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CalendarDlg)
{
  ui->setupUi(this);
  auto flags = this->windowFlags();
  flags &= ~Qt::WindowContextHelpButtonHint;
  QObject::connect(ui->calendarWidget,
                   SIGNAL(activated(const QDate&)),
                   this,
                   SLOT(updateDate(const QDate&)));
}

CalendarDlg::~CalendarDlg()
{
  delete ui;
}

void CalendarDlg::updateDate(const QDate & date)
{
  m_date = date;
  accept();
}
