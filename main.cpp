#include "mainwindow.h"
#include "logindialog.h"
#include "databaseutils.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDir>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
  QString error;
  bool bSuccess = BaitaSQL::open(db,
                                 qApp->applicationDirPath() +
                                 "/" +
                                 "setup.db",
                                 error);
  if (bSuccess)
    bSuccess = BaitaSQL::init(db, error);
  else
    db.close();

  if (!bSuccess)
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       QObject::tr("Erro ao inicializar banco de dados"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
  }
  else
  {
    UserLoginSQL userLogin;
    userLogin.setDatabase(db);
    LoginDialog l(userLogin);
    if (l.exec() == QDialog::Accepted)
    {
      BaitaAssistant w(userLogin);
      w.init();
      w.show();
      return a.exec();
    }
  }
  return 0;
}
