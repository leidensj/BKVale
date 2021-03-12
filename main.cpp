#include "mainwindow.h"
#include "controls/logindialog.h"
#include "databaseutils.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QString>
#include <QObject>
#include "controls/couponredeemer.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

   QSqlDatabase::addDatabase("QPSQL", POSTGRE_CONNECTION_NAME);
  if (argc == 4 && strcmp(argv[1], "redeemer") == 0)
  {
    QString server = argv[2] == nullptr ? "localhost" : argv[2];
    int port = argv[3] == nullptr ? 5432 : QString(argv[3]).toInt();
    QString error;
    if (BaitaSQL::open(server, port, error))
    {
      CouponRedeemer w;
      w.show();
      return a.exec();
    }
    else
    {
      QMessageBox::critical(nullptr,
                            QObject::tr("Baita"),
                            QObject::tr("Erro ao iniciar banco de dados.\nServidor: %1\nPorta: %2\nErro: %3").arg(argv[2], argv[3], error));
      return -1;
    }
  }
  else
  {
    LoginDialog l;
    if (l.exec() == QDialog::Accepted)
    {
      Baita w;
      w.show();
      return a.exec();
    }
  }
  return 0;
}
