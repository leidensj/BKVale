#include "mainwindow.h"
#include "logindialog.h"
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
  ActiveUser login;
  LoginDialog l(login);
  if (l.exec() == QDialog::Accepted)
  {
    Tipi w(login);
    w.show();
    return a.exec();
  }
  return 0;
}
