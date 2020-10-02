#include "mainwindow.h"
#include "controls/logindialog.h"
#include "databaseutils.h"
#include "settings.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDir>
#include <QFileDialog>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  Settings settings;
  settings.load();

  QSqlDatabase::addDatabase("QPSQL", POSTGRE_CONNECTION_NAME);
  LoginDialog l;
  if (l.exec() == QDialog::Accepted)
  {
    Goiabo w;
    w.show();
    return a.exec();
  }
  return 0;
}
