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
#include "views/cashclosingview.h"
#include "views/jitemview.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QCoreApplication::setApplicationVersion("2.1");

  QCommandLineParser parser;
  parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

  QCommandLineOption redeemerOpt("r", "Inicia a versão de resgate de códigos.");
  parser.addOption(redeemerOpt);
  QCommandLineOption cashclosingOpt("c", "Inicia a versão de fechamento de caixa.");
  parser.addOption(cashclosingOpt);
  QCommandLineOption serverOpt(QStringList() << "s", "Especifíca o servidor do banco de dados.", "server", "");
  parser.addOption(serverOpt);
  QCommandLineOption portOpt(QStringList() << "p", "Especifíca a porta do banco de dados.", "port", "5432");
  parser.addOption(portOpt);
  parser.process(a);

  bool bRedeemer = parser.isSet(redeemerOpt);
  bool bCashclosing = parser.isSet(cashclosingOpt);
  QString server = parser.value(serverOpt);
  QString port = parser.value(portOpt);

   QSqlDatabase::addDatabase("QPSQL", POSTGRE_CONNECTION_NAME);
  if (bRedeemer || bCashclosing)
  {
     QString error;
     Settings s;
     s.load();
     if (server.isEmpty())
       server = s.m_databaseHostName;
     if (server.isEmpty())
       server = "localhost";
     if (port.isEmpty())
       port.setNum(s.m_databasePort);
     if (port.isEmpty())
       port = "5432";
    if (BaitaSQL::open(server, port.toInt(), error))
    {
      if (bRedeemer)
      {
        CouponRedeemer w;
        w.setWindowTitle(QObject::tr("Baita Cupom"));
        w.setWindowIcon(QIcon(":/icons/res/coupon.png"));
        w.show();
        return a.exec();
      }
      else if (bCashclosing)
      {
        auto p = new CashClosingView;
        JItemViewDialog w(p);
        w.setWindowTitle(QObject::tr("Fechamento de Caixa"));
        w.setWindowIcon(QIcon(":/icons/res/cashier.png"));
        auto f = w.font();
        f.setPointSize(14);
        w.setFont(f);
        w.showMaximized();
        return a.exec();
      }
    }
    else
    {
      QMessageBox::critical(nullptr,
                            QObject::tr("Baita"),
                            QObject::tr("Erro ao iniciar banco de dados.\nServidor: %1\nPorta: %2\nErro: %3").arg(server, port, error));
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
