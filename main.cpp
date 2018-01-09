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
  BaitaAssistant w;
  QSqlDatabase db;
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
    w.close();
    QMessageBox msgBox(QMessageBox::Critical,
                       QObject::tr("Erro ao inicializar banco de dados"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
  }
  else
  {
    UserLoginSQL userLogin(db);
    LoginDialog l(userLogin);
    if (l.exec() == QDialog::Accepted)
    {
      w.init();
      w.show();
    }
  }

  return a.exec();
}
