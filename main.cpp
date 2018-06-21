#include "mainwindow.h"
#include "logindialog.h"
#include "databaseutils.h"
#include "settings.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDir>
#include <QFileDialog>

namespace
{
bool initDatabase(const QString& filePath,
                  QString& error)
{
  bool bSuccess = QSqlDatabase::database(SQLITE_CONNECTION_NAME).isValid();
  if (bSuccess)
  {
    bSuccess = BaitaSQL::open(filePath, error);
    if (bSuccess)
    {
      bSuccess = BaitaSQL::init(error);
      if (!bSuccess)
        QSqlDatabase::database(SQLITE_CONNECTION_NAME).close();
    }
  }
  return bSuccess;
}

QString getPath()
{
  return QFileDialog::getExistingDirectory(nullptr,
                                           QObject::tr("Selecionar local para salvar seus dados"),
                                           "/home",
                                           QFileDialog::ShowDirsOnly
                                           | QFileDialog::DontResolveSymlinks);
}
}


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  Settings settings;
  settings.load();
  if (settings.m_databaseDir.isEmpty())
    settings.m_databaseDir = getPath();

  if (settings.m_databaseDir.isEmpty())
    return 0;

  settings.save();
  QSqlDatabase::addDatabase("QSQLITE", SQLITE_CONNECTION_NAME);
  QString error;
  if (!initDatabase(settings.databasePath(), error))
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
    LoginDialog l(userLogin);
    if (l.exec() == QDialog::Accepted)
    {
      BaitaAssistant w(userLogin);
      w.show();
      return a.exec();
    }
  }
  return 0;
}
