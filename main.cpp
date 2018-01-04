#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  LoginDialog l;
  BaitaAssistant w;
  if (l.exec() == QDialog::Accepted)
    w.show();
  return a.exec();
}
