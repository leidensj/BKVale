#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "defines.h"

AboutDialog::AboutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);
  ui->lblVersion->setText(VERSION);
}

AboutDialog::~AboutDialog()
{
  delete ui;
}
