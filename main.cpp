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
  UserLoginSQL userLogin;
  LoginDialog l(userLogin);
  if (l.exec() == QDialog::Accepted)
  {
    BaitaAssistant w(userLogin);
    w.show();
    return a.exec();
  }
  return 0;
}
