#include "mainwindow.h"
#include "controls/logindialog.h"
#include "databaseutils.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QString>
#include <QObject>
#include <QCommandLineParser>
#include "controls/couponredeemer.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QCoreApplication::setApplicationVersion("2.1");

  QCommandLineParser parser;
  parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

  QCommandLineOption redeemerOpt("r", "Inicia a versão de resgate de códigos.");
  parser.addOption(redeemerOpt);
  QCommandLineOption serverOpt(QStringList() << "s", "Especifíca o servidor do banco de dados.", "server", "");
  parser.addOption(serverOpt);
  QCommandLineOption portOpt(QStringList() << "p", "Especifíca a porta do banco de dados.", "port", "5432");
  parser.addOption(portOpt);
  parser.process(a);

  bool bRedeemer = parser.isSet(redeemerOpt);
  QString server = parser.value(serverOpt);
  int port = parser.value(portOpt).toInt();

   QSqlDatabase::addDatabase("QPSQL", POSTGRE_CONNECTION_NAME);
  if (bRedeemer)
  {
     QString error;
    if (BaitaSQL::open(server, port, error))
    {
      CouponRedeemer w;
      w.setWindowTitle(QObject::tr("Baita Cupom"));
      w.setWindowIcon(QIcon(":/icons/res/coupon.png"));
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
